/*
 * bss_data.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BSS_DATA_BSS_DATA_C_
#define COMPONENT_BSS_DATA_BSS_DATA_C_

#include "bss_data.h"

static void bss_data_serialize_impl(BSS_Data* p_bss_data, char* buff);

BSS_Data* bss_data_construct(void){
	BSS_Data* p_bss_data = (BSS_Data*)malloc(sizeof(BSS_Data));
	while(p_bss_data == NULL);
	p_bss_data->data_serialize = bss_data_serialize_impl;
	return p_bss_data;
}

static void bss_data_serialize_impl(BSS_Data* p_bss_data, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='S';
    *buff++=',';
	buff+=long_to_string(p_bss_data->serial_number,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->cab_num,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->active_cab_num,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->tilt_ss_state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->fan_state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->charger_state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->lamp_state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss_data->temp,buff);
    *buff++='*';
    *buff++='\0';
}

#endif /* COMPONENT_BSS_DATA_BSS_DATA_C_ */
