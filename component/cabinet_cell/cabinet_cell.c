/*
 * cabinet_cell.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_cell.h"

static void cab_cell_data_serialze_impl(Cabinet* p_cab, char* buff);

void cab_cell_init(Cabinet* p_cab){
	p_cab->data_serialize = cab_cell_data_serialze_impl;
}

void cab_cell_update_state(Cabinet* p_cab){
        CABINET_OP_STATE old_state = p_cab->op_state;
        CABINET_OP_STATE new_state = CAB_CELL_ST_EMPTY;
	switch(p_cab->bp->base.con_state){
	case CO_SLAVE_CON_ST_DISCONNECT:
		new_state = CAB_CELL_ST_EMPTY;
		break;
	case CO_SLAVE_CON_ST_ASSIGNING:
		new_state = CAB_CELL_ST_BP_ID_ASSIGN;
		break;
	case CO_SLAVE_CON_ST_AUTHORIZING:
		new_state = CAB_CELL_ST_BP_ID_AUTHORIZE;
		break;
	case CO_SLAVE_CON_ST_CONNECTED:
		if(old_state == CAB_CELL_ST_CHARGING){
			return;
		}
		new_state = CAB_CELL_ST_STANDBY;
		break;
	}
	if(new_state != old_state){
	        p_cab->op_state = new_state;
	        p_cab->is_changed = 1;
	}
}

void cab_cell_open_door(Cabinet* p_cab){
	cab_door_open(&p_cab->door);
}

void cab_cell_update_door_state(Cabinet* p_cab, DOOR_STATE new_state){
	DOOR_STATE old_state=p_cab->door.state;
	if(old_state != new_state){
		p_cab->door.state = new_state;
		p_cab->is_changed = 1;
	}
}

void cab_cell_update_fan_state(Cabinet* p_cab, SW_STATE new_state){
	SW_STATE old_state = p_cab->cell_fan.state;
	if(old_state != new_state){
		p_cab->door.state = new_state;
		p_cab->is_changed = 1;
	}
}

void cab_cell_update_temp(Cabinet* p_cab, uint8_t new_temp){
	uint8_t old_temp = p_cab->temp;
	if(old_temp != new_temp){
		p_cab->temp = new_temp;
		p_cab->is_changed = 1;
	}
}

static void cab_cell_data_serialze_impl(Cabinet* p_cab, char* buff){
	*buff++=':';
	*buff++='R';
    *buff++=',';
	*buff++='C';
    *buff++=',';
	buff+=long_to_string(p_cab->cab_id,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->op_state,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->door.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->cell_fan.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->temp,buff);
    *buff++=',';
    if(p_cab->bp->base.con_state==CO_SLAVE_CON_ST_CONNECTED){
        for(uint8_t i = 0; *(p_cab->bp->base.sn + i) != '\0'; i++){
        	*buff++= *(p_cab->bp->base.sn+i);
        }
    }
    else{
    	*buff++='0';
    }
    *buff++='*';
    *buff++='\n';
    *buff++='\0';
}
