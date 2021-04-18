/*
 * bat_pack.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "bp_data.h"

static void bp_data_serialize_impl(BP* p_bp, char* buff);

BP* bp_construct(void){
	BP* p_bp = (BP*)malloc(sizeof(BP));
	while(p_bp == NULL);
	p_bp->data_serialize = bp_data_serialize_impl;
	return p_bp;
}

void bp_update_state(BP* p_bp, BP_STATE state){

}

BP_STATE bp_get_state(BP* p_bp){
	return 0;
}

void bp_update_serial_number(BP* p_bp, char* sn){

}

char* bp_get_serial_number(BP* p_bp){
	return 0;
}

void bp_update_soc(BP* p_bp, uint8_t soc){

}

uint8_t bp_get_soc(BP* p_bp){
	return 0;
}

void bp_update_soh(BP* p_bp, uint8_t soh){

}

uint8_t bp_get_soh(BP* p_bp){
	return 0;
}

void bp_update_vol(BP* p_bp, uint8_t vol){

}

uint8_t bp_get_vol(BP* p_bp){
	return 0;
}

void bp_update_cur(BP* p_bp, uint8_t cur){

}

uint8_t bp_get_cur(BP* p_bp){
	return 0;
}

void bp_update_temp(BP* p_bp, uint16_t temp){

}

uint16_t bp_get_temp(BP* p_bp){
	return 0;
}

void bp_update_cycle(BP* p_bp, uint32_t cycle){

}

uint32_t bp_get_cycle(BP* p_bp){
	return 0;
}

static void bp_data_serialize_impl(BP* p_bp, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='B';
    *buff++=',';
	buff+=long_to_string(p_bp->serial_number,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->vol,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->cur,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->state,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->soc,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->soh,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->temp,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->cycle,buff);
    *buff++='*';
    *buff++='\0';
}
