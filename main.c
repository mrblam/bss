/*
 * test_com_hmi.c
 *
 *  Created on: Apr 21, 2021
 *      Author: Ha
 */

/*
 * test_sync_data.c
 *
 *  Created on: Apr 15, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "cabinet_app.h"
#include "app_config.h"
#include "peripheral_init.h"

char buff[50];
char s;
uint8_t idx, cnt, i, char_state, get_cmd_done;

Cabinet_App selex_bss_app;
static void cabinet_door_close_event_handle(Cabinet* p_cab);
static void cabinet_door_open_event_handle(Cabinet* p_cab);
static void can_master_slave_select_impl(const CAN_master* p_cm,const uint32_t id);
static void can_master_slave_deselect_impl(const CAN_master* p_cm,const uint32_t id);
static void bp_assign_id_success_handle(const CAN_master* const p_cm,const uint32_t id);
static void bp_assign_id_fail_handle(const CAN_master* const p_cm,const uint32_t id);
static void can_receive_handle(CAN_Hw* p_hw);


static Cabinet bss_cabinets[CABINET_CELL_NUM];
static CO_Slave* bp_slaves[CABINET_CELL_NUM];
static uint32_t sys_timestamp=0;
static uint32_t sys_tick_ms=0;

void cab_app_init(Cabinet_App* p_ca){
	p_ca->bss.cab_num=CABINET_CELL_NUM;
	p_ca->state = CABIN_ST_SETUP;
	p_ca->bss.cabs=&bss_cabinets[0];
	peripheral_init(p_ca);
	for(int i=0;i<CABINET_CELL_NUM;i++){
		bss_cabinets[i].state=CAB_CELL_ST_INACTIVE;
	        bss_cabinets[i].cab_id=i;
	        bss_cabinets[i].bp=bp_construct();
	        bss_cabinets[i].bp->pos=i;
	        bss_cabinets[i].bp->soc=90;
	        bss_cabinets[i].bp->soc=100;
	        bss_cabinets[i].bp->cycle=8;
	        bss_cabinets[i].bp->temp=26;
	        bss_cabinets[i].on_door_close=cabinet_door_close_event_handle;
	        bss_cabinets[i].on_door_open=cabinet_door_open_event_handle;
	        bss_cabinets[i].is_changed=1;
	        while(bss_cabinets[i].bp==NULL);
	        bp_slaves[i]=(CO_Slave*)(bss_cabinets[i].bp);
	        co_slave_set_con_state(bp_slaves[i],CO_SLAVE_CON_ST_DISCONNECT);
	        bp_slaves[i]->node_id=CABINET_START_NODE_ID+i;
	        bp_slaves[i]->sdo_server_address=0x580+bp_slaves[i]->node_id;
	        cabinet_init(&bss_cabinets[i]);
	        sw_off(&bss_cabinets[i].node_id_sw);
	}

	co_slave_set_con_state(bp_slaves[1], CO_SLAVE_CON_ST_CONNECTED);
	bss_cabinets[1].bp->soc=100;

	p_ca->base.slave_start_node_id=CABINET_START_NODE_ID;
	can_master_init((CAN_master*)p_ca,bp_slaves,CABINET_CELL_NUM,&can_port);
	p_ca->base.assign_state=CM_ASSIGN_ST_DONE;
	p_ca->base.slave_select=can_master_slave_select_impl;
	p_ca->base.slave_deselect=can_master_slave_deselect_impl;
	p_ca->base.on_slave_assign_fail=bp_assign_id_fail_handle;
	p_ca->base.on_slave_assign_success=bp_assign_id_success_handle;
	p_ca->base.sdo_server.rx_address=0x600+3;

	p_ca->ioe_cfan =&cell_fan;
	p_ca->ioe_sol = &solenoid;
	can_set_receive_handle(p_ca->base.p_hw, can_receive_handle);
}


int main (void){

	__disable_irq();
	board_init();
	cab_app_init(&selex_bss_app);
	bss_update_cabinets_state(&selex_bss_app.bss);
	cab_app_set_state(&selex_bss_app, CABIN_ST_STANDBY);
	uart_receives(&power_sys_port, &s);
	sys_tick_ms=1000/SYSTICK_FREQ_Hz;
	sys_timestamp=0;
	__enable_irq();
	can_master_start_assign_next_slave((CAN_master*)&selex_bss_app, sys_timestamp);
	while(1){
	};
}

static void can_receive_handle(CAN_Hw* p_hw){

	uint32_t cob_id=p_hw->can_rx.StdId;

	/* if assign request message */
	if (cob_id == selex_bss_app.base.node_id_scan_cobid) {
		if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_WAIT_REQUEST) {
			selex_bss_app.base.p_hw->can_tx.StdId = selex_bss_app.base.node_id_scan_cobid;
			selex_bss_app.base.p_hw->can_tx.DLC = 0;
			can_send(selex_bss_app.base.p_hw, selex_bss_app.base.p_hw->tx_data);
			selex_bss_app.base.assign_state=CM_ASSIGN_ST_START;
			//can_master_start_assign_next_slave((CAN_master*)&selex_bss_app,sys_timestamp);
		} else if(selex_bss_app.base.assign_state==CM_ASSIGN_ST_SLAVE_SELECT){
			selex_bss_app.base.assign_state=CM_ASSIGN_ST_SLAVE_SELECT_CONFIRM;
		} else if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_WAIT_CONFIRM) {
			/* slave confirm assign id success*/
			if (p_hw->rx_data[0] == selex_bss_app.base.assigning_slave->node_id) {
				/* finish assign for current slave and move to next slave */

				cm_start_authorize_slave((CAN_master*)&selex_bss_app, selex_bss_app.base.assigning_slave);
			}
		}
	}

	if(cob_id==selex_bss_app.base.sdo_server.rx_address){
		CO_memcpy(selex_bss_app.base.sdo_server.rx_msg_data, p_hw->rx_data,8);
		selex_bss_app.base.sdo_server.is_new_msg=1;
	}
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

	sys_timestamp+= sys_tick_ms;
	bss_update_cabinets_state(&selex_bss_app.bss);
	can_master_process((CAN_master*)&selex_bss_app, sys_timestamp);
	can_master_update_id_assign_process((CAN_master*)&selex_bss_app,sys_timestamp);
}

void USART1_IRQHandler(void){
	HAL_CHECK_COM_IRQ_REQUEST(&power_sys_port.uart_module);

	switch(char_state){
	case 0:
		if(s == ':'){
			char_state = 1;
		}
		break;
	case 1:
		if(s == 'W'){
			char_state = 2;
		}
		break;
	case 2:
		if(s == '*'){
			char_state = idx = 0;
			get_cmd_done = 1;
			break;
		}
		*(buff + idx) = s;
		idx++;
		break;
	default:
		break;
	}

	uart_receives(&power_sys_port, &s);
}

void HAL_HMI_PROCESS_DATA_IRQ(void){
        static uint8_t cab_id=0;
        static uint32_t alive_heartbeat_counter=0;

	//static uint32_t sync_counter=0;
	if(get_cmd_done == 1){
		cab_app_decode_cmd_hmi(&selex_bss_app, buff);
		get_cmd_done = 0;
	}
	cab_id++;
	if(cab_id>=selex_bss_app.bss.cab_num){
		cab_id=0;
	}
	for(uint8_t i=0;i<selex_bss_app.bss.cab_num;i++){
	        if(selex_bss_app.bss.cabs[i].is_changed==1){
	               cab_app_sync_bp_data_hmi(&selex_bss_app, i);
	               cab_app_sync_cab_data_hmi(&selex_bss_app, i);
	               selex_bss_app.bss.cabs[i].is_changed=0;
	               alive_heartbeat_counter=0;
	        }
	}
	alive_heartbeat_counter++;
	if(alive_heartbeat_counter>10){
               cab_app_sync_cab_data_hmi(&selex_bss_app, 0);
               alive_heartbeat_counter=0;
	}

	CHECK_TIM_IRQ_REQUEST(&hmi_timer);
}

static void cabinet_door_close_event_handle(Cabinet* p_cab){
	if(selex_bss_app.state==CABIN_ST_SETUP) return;
	bp_set_con_state(p_cab->bp,CO_SLAVE_CON_ST_ASSIGNING);
	can_master_start_assign_next_slave((CAN_master*)&selex_bss_app, sys_timestamp);
}

static void cabinet_door_open_event_handle(Cabinet* p_cab){
	/* ignore event during setup process*/
	if(selex_bss_app.state==CABIN_ST_SETUP) return;
	bp_set_con_state(p_cab->bp, CO_SLAVE_CON_ST_DISCONNECT);
	cab_cell_update_state(p_cab);
	sw_off(&p_cab->node_id_sw);

}

static void can_master_slave_select_impl(const CAN_master* p_cm,const uint32_t id){
	(void)p_cm;
	sw_off(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void can_master_slave_deselect_impl(const CAN_master* p_cm,const uint32_t id){
	(void)p_cm;
	sw_on(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void bp_assign_id_success_handle(const CAN_master* const p_cm,const uint32_t id){
	(void)p_cm;
	sw_on(&(selex_bss_app.bss.cabs[id].node_id_sw));
	//cab_app_active_charge(&selex_bss_app,id);
}

static void bp_assign_id_fail_handle(const CAN_master* const p_cm,const uint32_t id){

	(void)p_cm;
	/* return battery to user */
	cab_app_delivery_bp(&selex_bss_app, id);
	cab_cell_update_state(&selex_bss_app.bss.cabs[id]);
}

