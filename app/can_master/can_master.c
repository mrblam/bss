/*
 * can_master.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"

CAN_master* can_master_construct(void){
	CAN_master* p_cm = (CAN_master*)malloc(sizeof(CAN_master));
	while(p_cm == NULL);
	return p_cm;
}

void can_master_init(CAN_master* p_cm){
	p_cm->assign_state = ASSIGN_ST_INACTIVE;
	p_cm->empty_slave_list = list_init();
	for(uint8_t slave_id = SL1; slave_id <= SL15; slave_id++){
		list_insert_to_tail(p_cm->empty_slave_list, slave_id);
	}
}

void can_master_accept_assign_request(__attribute__((unused)) CAN_master* p_cm){
	can_port.can_tx.StdId = CO_CAN_ID_RSDO + MASTER_ID;
	can_send(&can_port,0);
}

uint8_t can_master_select_slave(CAN_master* p_cm){
	return list_walk_down(p_cm->empty_slave_list);
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
