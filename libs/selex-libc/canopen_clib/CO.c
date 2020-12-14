#include "../../../libs/selex-libc/canopen_clib/CO.h"

#include "stdlib.h"
#include "stdint.h"

#include "../../../libs/selex-libc/canopen_clib/CAN_Msg.h"

uint8_t SOM[]={0x55,0xAA};

static inline CAN_Msg* co_get_rx_msg(CO* p_dev);
static inline CAN_Msg* co_get_rx_msg_buffer_from_pool(CO* p_dev);
static inline void CO_SDO_process(CO* p_dev,CAN_Msg* p_msg);
static inline CAN_Msg* get_msg_from_buffer(CAN_Msg* buffer,
		const uint8_t buffer_size,const bool is_new);

CO* CO_create(const uint8_t node_id){

	CO* p_co=(CO*)malloc(sizeof(CO));
	while(p_co==NULL);
	p_co->node_id=node_id;
	return p_co;
}


void CO_set_od(CO* p_co,CO_OD* p_od){

	p_co->od=p_od;
}
void CO_process(CO* p_co){
       
       CAN_Msg* p_msg=co_get_rx_msg(p_co);
       while(p_msg !=NULL){

                CO_SDO_process(p_co,p_msg);

                 /* if msg is handled by sdos */
                if(p_msg->is_new==false){
                        p_msg=co_get_rx_msg(p_co);
                        continue;
                }
        
                /* message is unsupport */
                if(p_msg->is_new==true){
                        p_msg->is_new=false;
                        p_msg=co_get_rx_msg(p_co);
                }
       }
}

void CO_process_tpdo(CO* p_co){

        uint8_t pdo_id=0;
        for(pdo_id=0;pdo_id< TPDO_NUMBER;pdo_id++){
                pdo_handle((CO_PDO*)&p_co->tpdos[pdo_id]);
        }
}

void CO_process_rpdo(CO* p_co){

        uint8_t pdo_id=0;
        for(pdo_id=0;pdo_id< RPDO_NUMBER;pdo_id++){
                pdo_handle((CO_PDO*)&p_co->rpdos[pdo_id]);
        }
}

void CO_set_send_handle(CO* p_dev,CO_Sends sends){
	p_dev->sends=sends;
}

static inline void CO_SDO_process(CO* p_dev,CAN_Msg* p_msg){
       
       int sdo_id=0; 
       CO_SDO* p_sdo=NULL;
       for(sdo_id=0;sdo_id<SDO_NUMBER;sdo_id++){
                p_sdo= &p_dev->sdos[sdo_id];
                sdo_process(p_sdo,p_dev->od,0);
                co_sends(p_dev,&p_sdo->tx_msg);
                p_sdo->rx_msg->is_new=false;
       }
}

static inline CAN_Msg* co_get_rx_msg(CO* p_dev){
       
	return get_msg_from_buffer(&p_dev->rx_msgs[0],RX_MSG_BUFFER_SIZE,true);
}

static inline CAN_Msg* co_get_rx_msg_buffer_from_pool(CO* p_dev){
	return get_msg_from_buffer(&p_dev->rx_msgs[0],RX_MSG_BUFFER_SIZE,false);
}

static inline CAN_Msg* get_msg_from_buffer(CAN_Msg* buffer,const uint8_t buffer_size,const bool is_new){

	CAN_Msg* p_msg=NULL;
       int msg_id=0;
        for(msg_id=0;msg_id<buffer_size;msg_id++){
                p_msg=&buffer[msg_id];
                if(p_msg->is_new==is_new) return p_msg;
        }
        return NULL;
}

void co_receive_msg(CO* p_dev,CAN_Msg* p_msg){

	CAN_Msg* msg_slot= co_get_rx_msg_buffer_from_pool(p_dev);
	if(msg_slot==NULL) return;
	msg_slot->cob_id=p_msg->cob_id;
	msg_slot->data_len=p_msg->data_len;
	msg_slot->data[0]=p_msg->data[0];
	msg_slot->data[1]=p_msg->data[1];
	msg_slot->data[2]=p_msg->data[2];
	msg_slot->data[3]=p_msg->data[3];
	msg_slot->data[4]=p_msg->data[4];
	msg_slot->data[5]=p_msg->data[5];
	msg_slot->data[6]=p_msg->data[6];
	msg_slot->data[7]=p_msg->data[7];
	msg_slot->is_new=true;

	for(int i=0;i<RPDO_NUMBER;i++){
		if(p_dev->rpdos[i].base.cob_id==msg_slot->cob_id){
			pdo_set_msg((CO_PDO*)&p_dev->rpdos[i],msg_slot);
			return;
		}
	}

	for(int i=0;i<TPDO_NUMBER;i++){
		if(p_dev->tpdos[i].base.cob_id==msg_slot->cob_id){
			pdo_set_msg((CO_PDO*)&p_dev->tpdos[i],msg_slot);
			return;
		}
	}

	for(int i=0;i<SDO_NUMBER;i++){
		if(p_dev->sdos[i].client_server_id==msg_slot->cob_id){
			sdo_set_msg(&p_dev->sdos[i],msg_slot);
			return;
		}
	}


}

void co_send_boost_msg(CO* p_co){
}
