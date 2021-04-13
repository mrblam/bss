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
	p_cm->slave_id = DEFAULT_BP_ID;
	p_cm->assign_state = ASSIGN_ST_ERROR;
	p_cm->empty_slave_list = list_init();
	for(uint8_t slave_id = SL1; slave_id <= SL15; slave_id++){
		list_insert_to_tail(p_cm->empty_slave_list, slave_id);
	}
}

void can_master_active_node_id_pin(CAN_master* p_cm, uint8_t node_id_pin){
	//p_cm->active_node_id_pin(node_id_pin);
}

void can_master_deactive_node_id_pin(CAN_master* p_cm, uint8_t node_id_pin){

}

void can_master_response_id_assign_req(CAN_master* p_cm){

}

uint8_t can_master_select_empty_slave(CAN_master* p_cm){
	can_master_reset_assign_state(p_cm);
	return list_walk_down(p_cm->empty_slave_list);
}

void can_master_send_id_msg(CAN_master* p_cm){

}

void can_master_reset_assign_state(CAN_master* p_cm){
	p_cm->assign_state = ASSIGN_ST_ERROR;
}

ASSIGN_STATE can_master_wait_slave_confirm(CAN_master* p_cm){
	return (rand()%2);
}

void can_master_update_slave_list(CAN_master* p_cm){
	list_remove_node(p_cm->empty_slave_list, p_cm->empty_slave_list->p_temp->data);
}

