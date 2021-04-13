/*
 * bat_pack.h
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BAT_PACK_BAT_PACK_H_
#define COMPONENT_BAT_PACK_BAT_PACK_H_

#include "stdint.h"
#include "stdlib.h"

typedef struct BP_t BP;

typedef enum BP_STATE{
	BP_ST_FAIL = 0,
	BP_ST_IDLE = 1,
	BP_ST_CHARGING = 2
} BP_STATE;

struct BP_t{
	char serial_number[32];
	BP_STATE state;
	uint8_t pos;
	uint8_t soc;
	uint8_t soh;
	uint8_t vol;
	uint8_t cur;
	uint16_t temp;
	uint32_t cycle;
};

BP* bp_construct(void);
void bp_init(BP* p_bp);

void bp_update_state(BP* p_bp, BP_STATE state);
BP_STATE bp_get_state(BP* p_bp);

void bp_update_serial_number(BP* p_bp, char* sn);
char* bp_get_serial_number(BP* p_bp);

void bp_update_pos(BP* p_bp, uint8_t pos);
uint8_t bp_get_pos(BP* p_bp);

void bp_update_soc(BP* p_bp, uint8_t soc);
uint8_t bp_get_soc(BP* p_bp);

void bp_update_soh(BP* p_bp, uint8_t soh);
uint8_t bp_get_soh(BP* p_bp);

void bp_update_vol(BP* p_bp, uint8_t vol);
uint8_t bp_get_vol(BP* p_bp);

void bp_update_cur(BP* p_bp, uint8_t cur);
uint8_t bp_get_cur(BP* p_bp);

void bp_update_temp(BP* p_bp, uint16_t temp);
uint16_t bp_get_temp(BP* p_bp);

void bp_update_cycle(BP* p_bp, uint32_t cycle);
uint32_t bp_get_cycle(BP* p_bp);

#endif /* COMPONENT_BAT_PACK_BAT_PACK_H_ */
