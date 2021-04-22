/*
 * cabinet_cell.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_cell.h"

static void cab_cell_data_serialze_impl(Cabinet_Cell* p_cc, char* buff);

Cabinet_Cell* cab_cell_construct(void){
	Cabinet_Cell* p_cc = (Cabinet_Cell*)malloc(sizeof(Cabinet_Cell));
	while(p_cc == NULL);

	p_cc->bp = bp_construct();
	p_cc->state = CAB_CELL_ST_EMPTY;
	p_cc->op_state = CAB_CELL_OP_ST_INACTIVE;
	p_cc->node_id = BP_DEFAULT_CAN_NODE_ID;
	p_cc->temp = 25;
	p_cc->bp = bp_construct();
	p_cc->cell_fan = sw_construct();
	p_cc->charger = sw_construct();
	p_cc->door = door_construct();
	p_cc->data_serialize = cab_cell_data_serialze_impl;
	return p_cc;
}

uint8_t cab_cell_get_slave_id(Cabinet_Cell* p_cc){
	return 1;
}

void cab_cell_open_door(Cabinet_Cell* p_cc){
	cab_door_open(p_cc->door);
}

void cab_cell_check_door_state(Cabinet_Cell* p_cc){

}

void cab_cell_update_cell_temp(Cabinet_Cell* p_cc){
	ntc_update_temp(p_cc->temp_ss);
}

int32_t cab_cell_get_temp(Cabinet_Cell* p_cc){
	return 0;
}

void cab_cell_fan_turn_on(Cabinet_Cell* p_cc){
	sw_on(p_cc->cell_fan);
}

void cab_cell_fan_turn_off(Cabinet_Cell* p_cc){
	sw_off(p_cc->cell_fan);
}

void cab_cell_active_charger(Cabinet_Cell* p_cc){
	sw_on(p_cc->charger);
}

void cab_cell_deactive_charger(Cabinet_Cell* p_cc){
	sw_off(p_cc->charger);
}

static void cab_cell_data_serialze_impl(Cabinet_Cell* p_cc, char* buff){
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
    if(p_cc->bp){
        for(uint8_t i = 0; *(p_cc->bp->serial_number + i) != '\0'; i++){
        	*buff++= *(p_cc->bp->serial_number + i);
        }
    }
    else *buff++='0';
    *buff++='*';
    *buff++='\0';
}
