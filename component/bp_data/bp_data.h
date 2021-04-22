/*
 * bp_data.h
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BP_DATA_BP_DATA_H_
#define COMPONENT_BP_DATA_BP_DATA_H_

#include "stdint.h"
#include "stdlib.h"
#include "string_util.h"
#include "CO.h"

typedef struct BP_t BP;

typedef enum BP_STATE{
	BP_ST_FAIL = 0,
	BP_ST_IDLE = 1,
	BP_ST_CHARGING = 2
} BP_STATE;

typedef enum BP_CONNECT_STATE_t{

        BP_CON_ST_DISCONNECTED=0,
        BP_CON_ST_AUTHORIZING,
        BP_CON_ST_AUTHORIZED
}BP_CONNECT_STATE;

struct BP_t{
        CO_Slave base;
	char 	serial_number[32];
	BP_STATE 	state;
	BP_CONNECT_STATE con_state;
	uint32_t inactive_time;
	uint8_t 	pos;
	uint8_t 	soc;
	uint8_t 	soh;
	uint8_t 	vol;
	uint8_t 	cur;
	uint16_t 	temp;
	uint32_t 	cycle;
	uint32_t        can_node_id;
	void		(*data_serialize)(BP* p_bp, char* buff);
};

BP* bp_construct(void);

void bp_update_state(BP* p_bp, BP_STATE state);
BP_STATE bp_get_state(BP* p_bp);

void bp_set_serial_number(BP* p_bp, char* sn);
char* bp_get_serial_number(BP* p_bp);

void bp_set_soc(BP* p_bp, uint8_t soc);
uint8_t bp_get_soc(BP* p_bp);

void bp_set_soh(BP* p_bp, uint8_t soh);
uint8_t bp_get_soh(BP* p_bp);

void bp_set_vol(BP* p_bp, uint8_t vol);
uint8_t bp_get_vol(BP* p_bp);

void bp_set_cur(BP* p_bp, uint8_t cur);
uint8_t bp_get_cur(BP* p_bp);

void bp_set_temp(BP* p_bp, uint16_t temp);
uint16_t bp_get_temp(BP* p_bp);

void bp_set_cycle(BP* p_bp, uint32_t cycle);
uint32_t bp_get_cycle(BP* p_bp);

static inline BP_CONNECT_STATE bp_get_con_state(const BP* const p_bp){
        return p_bp->con_state;
}

static inline void bp_set_con_state(BP* p_bp,const BP_CONNECT_STATE state){
        p_bp->con_state=state;
}

static inline void bp_reset_inactive_counter(BP* p_bp){
        p_bp->inactive_time=0;
}

static inline void bp_data_serialize(BP* p_bp, char* buff){
	p_bp->data_serialize(p_bp, buff);
}

#endif /* COMPONENT_BP_DATA_BP_DATA_H_ */
