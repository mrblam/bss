/*
 * bss_data.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BSS_DATA_BSS_DATA_C_
#define COMPONENT_BSS_DATA_BSS_DATA_C_

#include "bss_data.h"

static void bss_update_io_state(BSS_Data* p_bss);
static void bss_data_serialize_impl(BSS_Data* p_bss, char* buff);

void bss_init(BSS_Data* p_bss){
	p_bss->state = BSS_ST_INIT;
	p_bss->data_serialize = bss_data_serialize_impl;
	p_bss->bss_temps[0] = p_bss->bss_temps[1] = 30;

}

void bss_set_state(BSS_Data* p_bss, BSS_STATE new_state){
	BSS_STATE old_state = p_bss->state;
	if(old_state != new_state){
		p_bss->state = new_state;
		p_bss->is_changed = 1;
	}

	switch(p_bss->state){
	case BSS_ST_MAINTAIN:
		for(uint8_t i = 0; i < p_bss->cab_num; i++){
			if(p_bss->cabs[i].op_state != CAB_CELL_ST_INACTIVE){
				cab_cell_reset(&p_bss->cabs[i]);
				p_bss->cabs[i].is_changed = 1;
			}
		}
		break;
	case BSS_ST_ACTIVE:
		break;
	case BSS_ST_FAIL:
		break;
	default:
		break;
	}
}

void bss_update_cabinets_state(BSS_Data* p_bss){
	for(uint8_t i = 0; i < p_bss->cab_num; i++){
		if(p_bss->cabs[i].op_state != CAB_CELL_ST_INACTIVE){
			cab_cell_update_state(&p_bss->cabs[i]);
		}
	}
	//cab_cell_update_door_state(&p_bss->cabs[id], (DOOR_STATE)io_get_state(&p_bss->cabs[id].door.io_state));
}

static void bss_update_io_state(BSS_Data* p_bss){

	for(uint8_t i=0;i<p_bss->cab_num;i++){
		if(p_bss->cabs[i].op_state != CAB_CELL_ST_INACTIVE){
			cab_cell_update_door_state(&p_bss->cabs[i], (DOOR_STATE)io_get_state(&p_bss->cabs[i].door.io_state));
		}
	}
}

static void bss_data_serialize_impl(BSS_Data* p_bss, char* buff){
	*buff++=':';
	*buff++='R';
    *buff++=',';
	*buff++='S';
    *buff++=',';
	buff+=long_to_string(p_bss->cab_num,buff);
    *buff++=',';
	buff+=long_to_string(p_bss->state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss->tilt_ss.state,buff);
    *buff++=',';
    // Chargers
    *buff++='[';
    for(uint8_t i = 0; i < 2; i++){
    	buff+=long_to_string(p_bss->ac_chargers[i].input_power.state, buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++=',';
    // BSS_Fans
    *buff++='[';
    for(uint8_t i = 0; i < 2; i++){
    	buff+=long_to_string(p_bss->bss_fans[i].state, buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++=',';
    // BSS_Lamps
    *buff++='[';
    for(uint8_t i = 0; i < 4; i++){
    	buff+=long_to_string(p_bss->bss_lamps[i].state, buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++=',';
    // BSS_temps
    *buff++='[';
    for(uint8_t i = 0; i < 2; i++){
    	buff+=long_to_string(p_bss->bss_temps[i], buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++='*';
    *buff++='\0';
    *buff++='\n';
}

#endif /* COMPONENT_BSS_DATA_BSS_DATA_C_ */
