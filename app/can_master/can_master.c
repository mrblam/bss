/*
 * can_master.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"
static void can_master_assign_request_ack(CAN_master* p_cm);
static void can_master_select_slave(CAN_master* p_cm,CO_Slave* slave);

CAN_master* can_master_construct(void){
	CAN_master* p_cm = (CAN_master*)malloc(sizeof(CAN_master));
	while(p_cm == NULL);
	return p_cm;
}

void can_master_init(CO_Slave** slaves,CAN_Hw* p_hw,CAN_master* p_cm){
	p_cm->slaves=slaves;
	p_cm->p_hw=p_hw;
	p_cm->sdo_server.state=SDO_ST_IDLE;
}

CO_Slave* can_master_get_assign_request_slave(const CAN_master* const p_cm){
	for(int i=0;i<p_cm->slave_num;i++){
		if(p_cm->slaves[i]->con_state==CO_SLAVE_CON_ST_UNASSIGNED){
			return p_cm->slaves[i];
		}
	}
	return NULL;
}

void can_master_process(CAN_master* p_cm,const uint32_t timestamp){

		switch(p_cm->assign_state){
		case ASSIGN_ST_START:
			if(p_cm->sdo_server.state!=SDO_ST_SENT){
				can_master_select_slave(p_cm, p_cm->assigning_slave);
				p_cm->sdo_server.server_address=RSDO_ID;
				p_cm->sdo_server.object_mux=0x222;
				p_cm->sdo_server.state=SDO_ST_SENT;
				p_cm->sdo_server.timeout=timestamp+200;
			}else if(p_cm->sdo_server==SDO_ST_FAIL){
				p_cm->assigning_slave->con_state=CO_SLAVE_CON_ST_DISCONNECT;
				p_cm->assigning_slave=can_master_get_assign_request_slave(p_cm);
				if(p_cm->assigning_slave==NULL){
					p_cm->assign_state=ASSIGN_ST_DONE;
				}
			}
			else if(p_cm->sdo_server.state==SDO_ST_SUCCESS){
				p_cm->assigning_slave->con_state=CO_SLAVE_CON_ST_CONNECTED;
				p_cm->assigning_slave=can_master_get_assign_request_slave(p_cm);
				if(p_cm->assigning_slave==NULL){
					p_cm->assign_state=ASSIGN_ST_DONE;
				}
			}

			break;
		}

		if(p_cm->p_hw->can_rx.StdId==p_cm->sdo_server.tx_address){
			p_cm->sdo_server.state=SDO_ST_IDLE;
		}


	/* if assign request */
	if(p_cm->p_hw->can_rx.StdId==TSDO_ID){
		if(p_cm->assign_state!=ASSIGN_ST_DONE){
			p_cm->p_hw->can_tx.StdId=RSDO_ID;
			can_send(p_cm->p_hw, buff);
			p_cm->assigning_slave=can_master_get_assign_request_slave(p_cm);
			p_cm->assign_state=ASSIGN_ST_START;
		}
	}

	if(p_cm->p_hw->can_rx.StdId==p_cm->sdo_server.rx_address){
		can_master_process_sdo(p_cm, timestamp);
	}
}

void can_master_process_sdo(CAN_master* p_cm,const uint32_t timestamp){

	if(p_cm->sdo_server.timeout>=timestamp){
		p_cm->sdo_server.state=SDO_ST_FAIL;
		return;
	}
}

void can_master_start_assign_node_id(CAN_master* p_cm,const uint32_t slave_id){
	CO_Slave* slave=&p_cm->slaves[slave_id];
	can_master_select_slave(p_cm, slave);
}


void can_master_on_slave_assign_request(CAN_master* p_cm){
	can_master_accept_assign_request(p_cm);
}

static void can_master_assign_request_ack(CAN_master* p_cm){
	p_cm->p_hw->can_tx.StdId = CO_CAN_ID_RSDO + MASTER_ID;
	can_send(p_cm->p_hw,0);
}

static void can_master_select_slave(CAN_master* p_cm,CO_Slave* slave){
}

void can_master_send_id_msg(CAN_master* p_cm, uint8_t cab_id){
	can_master_deactive_node_id_pin(p_cm, cab_id);

	can_port.can_tx.StdId = CO_CAN_ID_RSDO + MASTER_ID;
	can_port.can_tx.DLC = 1;
	*can_port.tx_data = p_cm->empty_slave_list->p_temp->data;
	can_send(&can_port, can_port.tx_data);
	can_master_active_timing_state(p_cm, 10);
}

void can_master_active_timing_state(CAN_master* p_cm, uint16_t time_ms){
	p_cm->timing_state = TIMING_ST_ACTIVE;
	p_cm->time_stamp = time_ms;
}

void can_master_deactive_timing_state(CAN_master* p_cm){
	p_cm->timing_state = TIMING_ST_DEACTIVE;
	p_cm->time_stamp = 0;
}

void can_master_update_slave_list(CAN_master* p_cm){
	list_remove_node(p_cm->empty_slave_list, p_cm->empty_slave_list->p_temp->data);
}

void can_master_request_read_bp_sn(CAN_master* p_cm, uint8_t cab_id){
	can_port.can_tx.StdId = CO_CAN_ID_RSDO + cab_id;
	can_port.can_tx.DLC = 1;
	*can_port.tx_data = 's';
	can_send(&can_port, can_port.tx_data);
}

void can_master_send_sync_request(CAN_master* p_cm,const uint32_t timestamp){
#if 0
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
#endif
}
