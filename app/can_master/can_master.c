/*
 * can_master.c
 *
 *  Created on: Mar 25, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"

CAN_Master* cm_construct(void){
	CAN_Master* p_cm = (CAN_Master*)malloc(sizeof(CAN_Master));
	return p_cm;
}

void cm_init(CAN_Master* p_cm){
	p_cm->slave_number = SLAVE_NUMBER;
	p_cm->slave_id = NODE_MASTER;
	p_cm->empty_slave = list_init();
	for(uint8_t data = SLAVE1; data <= SLAVE15; data++){
		list_insert_to_tail(p_cm->empty_slave, data);
	}
	p_cm->busy_slave = list_init();
}

void cm_respone_assign_request(CAN_Master* p_cm){

}

void cm_send_assign_msg(CAN_Master* p_cm){

}

SLAVE_STATE cm_wait_slave_confirm(CAN_Master* p_cm){
	return SLAVE_OK;
}

SLAVE_STATE cm_select_slave(CAN_Master* p_cm, LIST* p_list){
	p_cm->slave_id = list_walk_down(p_list);
	if(p_cm->slave_id == END_OF_LIST) return SLAVE_ERROR;
	else return SLAVE_OK;
}

void cm_deselect_slave(CAN_Master* p_cm, uint8_t slave_id){
	list_remove_node(p_cm->empty_slave, slave_id);
	list_insert_to_tail(p_cm->busy_slave, slave_id);
}
