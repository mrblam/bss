/*
 * bat_pack.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "bat_pack.h"

BP* bp_construct(void){
	BP* p_bp = (BP*)malloc(sizeof(BP));
	while(p_bp == NULL);
	return p_bp;
}

void bp_init(BP* p_bp){
	p_bp->state = BP_ST_IDLE;
	p_bp->pos = 0;
	p_bp->soc = 0;
	p_bp->soh = 0;
	p_bp->vol = 0;
	p_bp->cur = 0;
	p_bp->temp = 0;
	p_bp->cycle = 0;
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

void bp_update_pos(BP* p_bp, uint8_t pos){

}

uint8_t bp_get_pos(BP* p_bp){
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
