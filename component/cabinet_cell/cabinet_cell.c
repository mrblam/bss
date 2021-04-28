/*
 * cabinet_cell.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_cell.h"

static void cab_cell_data_serialze_impl(Cabinet* p_cc, char* buff);

void cabinet_init(Cabinet* p_cc){
	p_cc->data_serialize=cab_cell_data_serialze_impl;
}

void cab_cell_update_state(Cabinet* p_cab){
	switch(p_cab->bp->base.con_state){
	case CO_SLAVE_CON_ST_DISCONNECT:
		p_cab->state=CAB_CELL_ST_EMPTY;
		break;
	case CO_SLAVE_CON_ST_ASSIGNING:
		p_cab->state=CAB_CELL_ST_BP_ID_ASSIGN;
		break;
	case CO_SLAVE_CON_ST_AUTHORIZING:
		p_cab->state=CAB_CELL_ST_BP_ID_AUTHORIZE;
		break;
	case CO_SLAVE_CON_ST_CONNECTED:
		p_cab->state=CAB_CELL_ST_ST_STANDBY;
		break;
	}
}

void cab_cell_open_door(Cabinet* p_cc){
	cab_door_open(&p_cc->door);
}

void cab_cell_update_door_state(Cabinet* p_cc){
	IO_STATE old_state=p_cc->door.io_state.state;
	IO_STATE new_state=p_cc->door.io_state.get_io_state(&p_cc->door.io_state);

	if(old_state != new_state){
		p_cc->door.io_state.state=new_state;
		p_cc->door.state=(DOOR_STATE)new_state;
		if((old_state==IO_ST_OFF) && (new_state==IO_ST_ON)){
			if(p_cc->on_door_close!=NULL){
				p_cc->on_door_close(p_cc);
			}
		}else if((old_state==IO_ST_ON)&&(new_state==IO_ST_OFF)){
			if(p_cc->on_door_open!=NULL){
				p_cc->on_door_open(p_cc);
			}
		}
	}
}

void cab_cell_update_cell_temp(Cabinet* p_cc){
	ntc_update_temp(p_cc->temp_ss);
}

int32_t cab_cell_get_temp(Cabinet* p_cc){
	return 0;
}

void cab_cell_fan_turn_on(Cabinet* p_cc){
	sw_on(&p_cc->cell_fan);
}

void cab_cell_fan_turn_off(Cabinet* p_cc){
	sw_off(&p_cc->cell_fan);
}

void cab_cell_active_charger(Cabinet* p_cc){
	sw_on(&p_cc->charger);
}

void cab_cell_deactive_charger(Cabinet* p_cc){
	sw_off(&p_cc->charger);
}

static void cab_cell_data_serialze_impl(Cabinet* p_cc, char* buff){

	*buff++=':';
	*buff++='R';
	*buff++='C';
    *buff++=',';
	buff+=long_to_string(p_cc->cab_id,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->door.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->cell_fan.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->temp,buff);
    *buff++=',';
    if(p_cc->bp->base.con_state==CO_SLAVE_CON_ST_CONNECTED){
        for(uint8_t i = 0; *(p_cc->bp->base.sn + i) != '\0'; i++){
        	*buff++= *(p_cc->bp->base.sn+i);
        }
    }
    else{
    	*buff++='0';
    }
    *buff++='*';
    *buff++='\n';
    *buff++='\0';
}
