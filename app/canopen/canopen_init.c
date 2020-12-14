/*
 * canopen_init.c
 *
 *  Created on: Oct 21, 2020
 *      Author: quangnd
 */

#include "canopen_init.h"
#include "CO.h"

typedef void (*CAN_Receive_Handle)(CAN_Hw* p_hw);

#define cbox_node_id                                            5

#define BP_VOL_CUR_TPDO_COBID                           CO_CAN_ID_TPDO_1
#define BP_LOW_CELLS_VOL_TPDO_COBID                     CO_CAN_ID_TPDO_2
#define BP_HIGH_CELLS_VOL_TPDO_COBID                    CO_CAN_ID_TPDO_3
#define BP_TEMP_TPDO_COBID                              CO_CAN_ID_TPDO_4


static void cbox_can_receive_handle_impl(CAN_Hw* p_hw);
static void rpdo_process(CAN_Hw* p_hw);
static void cbox_sdo_process(CAN_Hw* p_hw);
static CanTxMsg tx_msg;


BP_PDO_Data bp_data[BP_NUM];
VEH_PDO_Data veh_data;
static const char* default_serial_number="SBP_TEST2";

void canopen_service_init(void){

        can_set_receive_handle(&can1,cbox_can_receive_handle_impl);
        for(int i=0;i<BP_NUM;i++){
                for(int j=0;j<8;j++){
                        bp_data[i].temp[j]=25;
                }
                for(int j=0;j<16;j++){
                        bp_data[i].cell_vol[j]=32;
                }
                char* sn=(char*)default_serial_number;
                char* sn_buff=bp_data[i].serial_number;
                while(*sn){
                        *sn_buff++=*sn++;
                }
                bp_data[i].node_id=i+1;
                bp_data[i].vol=0;
                bp_data[i].cur=0;
                bp_data[i].soc=0;
                bp_data[i].soh=0;
                bp_data[i].state=0;
                bp_data[i].con_state=BP_CON_ST_DISCONNECTED;
                bp_data[i].inactive_time=0;
                bp_data[i].sdo_server.state=SDO_ST_IDLE;
                bp_data[i].sdo_server.receive_index=0;
        }
}

void bp_sdo_server_check_timeout(BP_PDO_Data* p_bp,const uint32_t timestamp){

        if(sdo_server_get_state(&p_bp->sdo_server)==SDO_ST_IDLE) {return;}
       /* request timeout  */
        if(p_bp->sdo_server.timeout< timestamp){
                sdo_server_set_state(&p_bp->sdo_server,SDO_ST_IDLE);
                bp_set_con_state(p_bp,BP_CON_ST_DISCONNECTED);
        }
}


static inline void bp_reset_inactive_counter(BP_PDO_Data* p_bp){
        p_bp->inactive_time=0;
}

void bp_send_sync_request(BP_PDO_Data *p_bp, const uint32_t timestamp) {

        /* sdo server is currently busy */
        if (sdo_server_get_state(&p_bp->sdo_server) !=SDO_ST_IDLE) {
                return;
        }

        if (bp_get_con_state(p_bp) == BP_CON_ST_DISCONNECTED) {
                /* start request serial number , reset receive index */
                p_bp->sdo_server.receive_index = 0;
                for (int i = 0; i < BP_SN_SIZE; i++) {
                        p_bp->serial_number[i] = '\0';
                }
                tx_msg.DLC = 0;
        } else {
                tx_msg.DLC = 1;
        }
        tx_msg.StdId = CO_CAN_ID_RSDO + p_bp->node_id;
        can_send(&can1, &tx_msg);
        sdo_server_set_state(&p_bp->sdo_server, SDO_ST_SENT);
        p_bp->sdo_server.timeout = timestamp + 500; /* timeout 500mS*/
        bp_set_con_state(p_bp, BP_CON_ST_AUTHORIZING);
}

static void cbox_can_receive_handle_impl(CAN_Hw* p_hw){

        uint32_t cob_id= p_hw->rx_msg.StdId & 0xFFFFFF80;

        switch(cob_id){
        case CO_CAN_ID_TPDO_1:
        case CO_CAN_ID_TPDO_2:
        case CO_CAN_ID_TPDO_3:
        case CO_CAN_ID_TPDO_4:
                rpdo_process(p_hw);
                break;
        case CO_CAN_ID_RSDO:
                cbox_sdo_process(p_hw);
                break;
        default:
                break;
        }
}

static void rpdo_process(CAN_Hw* p_hw){

        uint32_t cob_id= p_hw->rx_msg.StdId & 0xFFFFFF80;
        uint8_t node_id=(uint8_t)(p_hw->rx_msg.StdId & 0x7F);
        uint8_t bp_id=node_id-1;
        if(bp_get_con_state(&bp_data[bp_id]) != BP_CON_ST_AUTHORIZED){
                return;
        }

        bp_reset_inactive_counter(&bp_data[bp_id]);
        switch(cob_id){
        case BP_VOL_CUR_TPDO_COBID:
                bp_data[bp_id].vol=10*(uint32_t)CO_getUint16(p_hw->rx_msg.Data);
                bp_data[bp_id].cur=(int32_t)10*((int16_t)CO_getUint16(p_hw->rx_msg.Data+2));
                bp_data[bp_id].soc=p_hw->rx_msg.Data[4];
                bp_data[bp_id].state=p_hw->rx_msg.Data[5];
                bp_data[bp_id].status=(uint16_t)CO_getUint16(p_hw->rx_msg.Data+6);
                break;
        case BP_LOW_CELLS_VOL_TPDO_COBID:
                CO_memcpy(bp_data[bp_id].cell_vol,p_hw->rx_msg.Data,8);
                break;
        case BP_HIGH_CELLS_VOL_TPDO_COBID:
                CO_memcpy(bp_data[bp_id].cell_vol+8,p_hw->rx_msg.Data,8);
                break;
        case BP_TEMP_TPDO_COBID:
                CO_memcpy((uint8_t*)bp_data[bp_id].temp,p_hw->rx_msg.Data,8);
                break;
        default:
                break;
        }
}

static void cbox_sdo_process(CAN_Hw* p_hw){

        uint8_t node_id=(uint8_t)(p_hw->rx_msg.StdId & 0x7F);
        uint8_t bp_id=node_id-1;
        CO_memcpy((uint8_t*)(bp_data[bp_id].serial_number+bp_data[bp_id].sdo_server.receive_index),
                        p_hw->rx_msg.Data,p_hw->rx_msg.DLC);
        bp_reset_inactive_counter(&bp_data[bp_id]);
        if(p_hw->rx_msg.DLC<8){
                bp_set_con_state(&bp_data[bp_id],BP_CON_ST_AUTHORIZED);
        }
        else{
                bp_set_con_state(&bp_data[bp_id],BP_CON_ST_AUTHORIZING);
                bp_data[bp_id].sdo_server.receive_index+=p_hw->rx_msg.DLC;
        }

        sdo_server_set_state(&(bp_data[bp_id].sdo_server),SDO_ST_IDLE);
}

