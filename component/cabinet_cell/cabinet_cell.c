/*
 * cabinet_cell.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_cell.h"

static void cab_cell_data_serialze_impl(Cabinet_cell* p_cc, char* buff);

Cabinet_cell* cab_cell_construct(void){
	Cabinet_cell* p_cc = (Cabinet_cell*)malloc(sizeof(Cabinet_cell));
	while(p_cc == NULL);

	p_cc->node_id = DEFAULT_BP_ID;
	p_cc->bp = bp_construct();
	p_cc->cell_fan = sw_construct();
	p_cc->door = door_construct();
	p_cc->data_serialize = cab_cell_data_serialze_impl;
	return p_cc;
}

uint8_t cab_cell_get_slave_id(Cabinet_cell* p_cc){
	return 1;
}

void cab_cell_open_door(Cabinet_cell* p_cc){

}

void cab_cell_check_door_state(Cabinet_cell* p_cc){

}

void cab_cell_update_cell_temp(Cabinet_cell* p_cc){
	ntc_update_temp(p_cc->temp_ss);
}

int32_t cab_cell_get_temp(Cabinet_cell* p_cc){
	return 0;
}

void cab_cell_active_cell_fan(Cabinet_cell* p_cc){

}

void cab_cell_deactive_cell_fan(Cabinet_cell* p_cc){

}

static void cab_cell_data_serialze_impl(Cabinet_cell* p_cc, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='C';
    *buff++=',';
	buff+=long_to_string(p_cc->cab_id,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->op_state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->door->state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->cell_fan->state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->temp,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->bp->serial_number,buff);
    *buff++='*';
    *buff++='\0';
}
