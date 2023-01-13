/*
 * cabinet_cell.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_cell.h"

static void cab_cell_data_serialze_impl(Cabinet* p_cab, char* buff);

void cab_cell_init(Cabinet* p_cab, uint8_t id){
	p_cab->op_state = CAB_CELL_ST_INIT;
	p_cab->cab_id = id;
	p_cab->is_changed = 0;
	p_cab->bp = bp_construct(id);
	p_cab->data_serialize = cab_cell_data_serialze_impl;
}

void cab_cell_update_state(Cabinet* p_cab){
        CABINET_OP_STATE old_state = p_cab->op_state;
        CABINET_OP_STATE new_state = old_state ;
	switch(p_cab->bp->base.con_state){
	case CO_SLAVE_CON_ST_DISCONNECT:
		if((old_state != CAB_CELL_ST_INACTIVE) && (old_state != CAB_CELL_ST_INIT)){
			new_state = CAB_CELL_ST_EMPTY;
		}
		break;
	case CO_SLAVE_CON_ST_ASSIGNING:
		new_state = CAB_CELL_ST_BP_ID_ASSIGN;
		break;
	case CO_SLAVE_CON_ST_AUTHORIZING:
		new_state = CAB_CELL_ST_BP_ID_AUTHORIZE;
		break;
	case CO_SLAVE_CON_ST_CONNECTED:
		if(old_state != CAB_CELL_ST_CHARGING){
			new_state = CAB_CELL_ST_STANDBY;
		}
		break;
	}

	if(new_state != old_state){
		p_cab->op_state = new_state;
	}
}

void cab_cell_update_io_state(Cabinet* p_cab){
	cab_cell_update_door_state(p_cab, (DOOR_STATE)io_get_state(&p_cab->door.io_state));

#if ENABLE_CELL_FAN
	if((p_cab->temp >= 45)
			&& (p_cab->temp < 101)) sw_on(&p_cab->cell_fan);
	else if(p_cab->temp <= 40){
		if(p_cab->cell_fan.state == SW_ST_ON) sw_off(&p_cab->cell_fan);
	}
#endif
}

void cab_cell_open_door(Cabinet* p_cab){
	cab_door_open(&p_cab->door);
	p_cab->door.state = DOOR_ST_OPEN;
}

void cab_cell_update_led_state(Cabinet* p_cab){
       if(p_cab->is_changed == 1){
               if(p_cab->door.state == DOOR_ST_CLOSE){
                       cab_cell_set_led_color(p_cab, RED);
                       cab_cell_set_led_color(p_cab, BLINK);
               }
               else if(p_cab->door.state == DOOR_ST_OPEN){
                       cab_cell_set_led_color(p_cab, BLUE);
                       cab_cell_set_led_color(p_cab, BLINK);
               }
               p_cab->is_changed = 0;
       }
}

void cab_cell_update_door_state(Cabinet* p_cab, DOOR_STATE new_state){
	DOOR_STATE old_state = p_cab->door.state;
	if(old_state != new_state){
		p_cab->door.state = new_state;
		p_cab->is_changed = 1;
	}
}

void cab_cell_reset(Cabinet* p_cab){
	sw_off(&p_cab->node_id_sw);
	bp_reset_data(p_cab->bp);
	p_cab->op_state = CAB_CELL_ST_EMPTY;
}

void cab_cell_reset_io(Cabinet* p_cab){
	sw_off(&p_cab->node_id_sw);
	sw_off(&p_cab->charger);
	cab_cell_set_led_color(p_cab, NONE);
}

static void cab_cell_data_serialze_impl(Cabinet* p_cab, char* buff){
	*buff++=':';
	*buff++='R';
    *buff++=',';
	*buff++='C';
    *buff++=',';
	buff+=long_to_string(p_cab->cab_id,buff);
    *buff++=',';
    *buff++='A';
    *buff++=',';
	buff+=long_to_string(p_cab->op_state,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->door.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->cell_fan.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cab->temp,buff);
    *buff++=',';
//|| p_cab->bp->base.con_state == CO_SLAVE_CON_ST_AUTHORIZING
    if(p_cab->bp->base.con_state == CO_SLAVE_CON_ST_CONNECTED || p_cab->bp->base.con_state == CO_SLAVE_CON_ST_AUTHORIZING){
        for(uint8_t i = 0; *(p_cab->bp->base.sn + i) != '\0'; i++){
        	*buff++= *(p_cab->bp->base.sn + strlen((const char*)p_cab->bp->base.sn) - 1 - i);
        }
    }
    else if(p_cab->bp->is_data_available){
        for(uint8_t i = 0; *(p_cab->bp->base.sn + i) != '\0'; i++){
        	*buff++= *(p_cab->bp->base.sn + strlen((const char*)p_cab->bp->base.sn) - 1 - i);
        }
    }
    else{
    	*buff++='0';
    }
    *buff++='*';
    *buff++='\n';
    *buff++='\0';
}
