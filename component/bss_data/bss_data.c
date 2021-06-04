/*
 * bss_data.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BSS_DATA_BSS_DATA_C_
#define COMPONENT_BSS_DATA_BSS_DATA_C_

#include "bss_data.h"

static const char* default_serial_number="Selex_BSS";

static void bss_update_io_state(BSS_Data* p_bss);
static void bss_data_serialize_impl(BSS_Data* p_bss_data, char* buff);

BSS_Data* bss_data_construct(void){
	BSS_Data* p_bss_data = (BSS_Data*)malloc(sizeof(BSS_Data));
	while(p_bss_data == NULL);

    char* sn=(char*)default_serial_number;
    char* sn_buff=p_bss_data->serial_number;
    while(*sn){
            *sn_buff++=*sn++;
    }
    *sn_buff++='\0';

    p_bss_data->cab_num = 0;
    p_bss_data->tilt_ss_state = TILT_SS_INACTIVE;
    //p_bss_data->fan_state = FAN_ST_INACTIVE;
    p_bss_data->lamp_state = LAMP_ST_ACTIVE;
    p_bss_data->temp = 0;
	p_bss_data->data_serialize = bss_data_serialize_impl;
	return p_bss_data;
}

void bss_update_cabinets_state(BSS_Data* p_bss){

	for(uint8_t i=0;i<p_bss->cab_num;i++){
		/* read battery state and update to owner cabinet */
		cab_cell_update_state(&p_bss->cabs[i]);
	}
	bss_update_io_state(p_bss);
}

static void bss_update_io_state(BSS_Data* p_bss){

	for(uint8_t i=0;i<p_bss->cab_num;i++){
		if(p_bss->cabs[i].state!= CAB_CELL_ST_INACTIVE){
			cab_cell_update_door_state(&p_bss->cabs[i]);
		}
	}
}

static void bss_data_serialize_impl(BSS_Data* p_bss_data, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='S';
    *buff++=',';
    for(uint8_t i = 0; *(p_bss_data->serial_number + i) != '\0'; i++){
    	*buff++= *(p_bss_data->serial_number + i);
    }
    *buff++=',';
	buff+=long_to_string(p_bss_data->cab_num,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->tilt_ss_state,buff);
    *buff++=',';
	//buff+=long_to_string(p_bss_data->fan_state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->chargers[0].state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->chargers[1].state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->lamp_state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->temp,buff);
    *buff++='*';
    *buff++='\0';
}

#endif /* COMPONENT_BSS_DATA_BSS_DATA_C_ */
