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
	BP_ST_INIT=0,
	BP_ST_IDLE,
	BP_ST_ID_ASSIGN_START,
	BP_ST_ID_ASSIGN_WAIT_CONFIRM,
	BP_ST_ID_ASSIGN_CONFIRMED,
	BP_ST_ID_ASSIGN_WAIT_SLAVE_SELECT,
	BP_ST_START_AUTHENTICATE,
	BP_ST_AUTHENTICATING,
	BP_ST_SOFTSTART,
	BP_ST_SYSTEM_BOOST_UP,
	BP_ST_STANDBY,
	BP_ST_DISCHARGING,
	BP_ST_CHARGING,
	BP_ST_FAULT,
	BP_ST_SHIPMODE
} BP_STATE;

typedef enum BP_STATUS{
	BP_STT_OK				= 0,
	BP_STT_OCD 				= 1,
	BP_STT_SCD 				= 2,
	BP_STT_OV 				= 4,
	BP_STT_UV 				= 8,
	BP_STT_OVRD_ALERT 		= 16,
	BP_STT_DEVICE_XREADY	= 32,
	BP_STT_RSVD 			= 64,
	BP_STT_CC_READY 		= 128
}BP_STATUS;

struct BP_t{
    CO_Slave 	base;
    BP_STATE	state;
    uint32_t 	charge_sw_state;
	uint8_t 	pos;
	uint8_t 	soc;
	uint8_t 	soh;
	uint32_t 	vol;
	int32_t 	cur;
	uint32_t 	cycle;
	uint8_t 	cell_vol[16];
	uint8_t 	temp[8];
	BP_STATUS 	status;
	uint8_t 	is_changed;
	void		(*data_serialize)(BP* p_bp, char* buff);
};

BP* bp_construct(void);

void bp_update_state(BP* p_bp, BP_STATE state);
BP_STATE bp_get_state(BP* p_bp);

static inline CO_SLAVE_NET_STATE bp_get_con_state(const BP* const p_bp){
        return p_bp->base.con_state;
}

static inline void bp_set_con_state(BP* p_bp,const CO_SLAVE_NET_STATE state){
        co_slave_set_con_state((CO_Slave*) p_bp, state);
}

static inline void bp_reset_inactive_counter(BP* p_bp){
        p_bp->base.inactive_time_ms=0;
}

static inline void bp_data_serialize(BP* p_bp, char* buff){
	p_bp->data_serialize(p_bp, buff);
}

#endif /* COMPONENT_BP_DATA_BP_DATA_H_ */
