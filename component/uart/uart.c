/*
 * uart.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "uart.h"

static char* bp_serialize(BP* p_bp, char* buff);
static char* cab_serialize(Cabinet* p_cc, char* buff);
static char* bss_serialize(BSS_UART_Data* p_bss, char* buff);

BP_UART_Data 	bp_data[CABINET_CELL_NUM];
CAB_UART_Data	cab_data[CABINET_CELL_NUM];
BSS_UART_Data	bss_data;

void uart_stream_init(void){
	for(uint8_t i = 0; i < CABINET_CELL_NUM; i++){
		bp_data[i].serializer = bp_serialize;
		cab_data[i].serializer = cab_serialize;
	}
	bss_data.serializer = bss_serialize;
}

static char* bp_serialize(BP* p_bp, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='B';
	buff+=long_to_string(p_bp->state,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->pos,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->soc,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->soh,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->vol,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->cur,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->temp,buff);
	*buff++=',';
	buff+=long_to_string(p_bp->cycle,buff);
	*buff++='*';
	*buff++='\0';
	return buff;
}

static char* cab_serialize(Cabinet* p_cc, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='C';
	buff+=long_to_string(p_cc->cab_id,buff);
	*buff++=',';
	buff+=long_to_string(p_cc->state,buff);
	*buff++=',';
	buff+=long_to_string(p_cc->door->io_state->state,buff);
	*buff++=',';
	buff+=long_to_string(p_cc->cell_fan->state,buff);
	*buff++=',';
	buff+=long_to_string(p_cc->temp[0],buff);
	*buff++='*';
	*buff++='\0';
	return buff;
}

static char* bss_serialize(BSS_UART_Data* p_bss, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='B';
	*buff++='S';
	buff+=long_to_string(p_bss->state,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->tilt_ss_state,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->fan_state,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->charger_state,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->temp,buff);
	*buff++='*';
	*buff++='\0';
	return buff;
}
