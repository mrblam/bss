/*
 * switch.h
 *
 *  Created on: Aug 21, 2020
 *      Author: quangnd
 */

#ifndef COMPONENT_SWITCH_SWITCH_H_
#define COMPONENT_SWITCH_SWITCH_H_

#include "stdlib.h"

typedef enum SW_STATE{
	SW_ST_OFF = 0,
	SW_ST_ON,
	SW_ST_FAIL
}SW_STATE;

typedef struct Switch_t Switch;
typedef void (*sw_act)(Switch* p_sw);

struct Switch_t{
	SW_STATE	state;
	sw_act 		sw_on;
	sw_act		sw_off;
};

Switch* sw_construct(void);

static inline void sw_on(Switch* p_sw){
	p_sw->sw_on(p_sw);
	//p_sw->state=SW_ST_ON;
}

static inline void sw_off(Switch* p_sw){
	p_sw->sw_off(p_sw);
	//p_sw->state=SW_ST_OFF;
}

static inline SW_STATE sw_get_state(Switch* p_sw){
	return p_sw->state;
}

static inline void sw_process(Switch* p_sw, SW_STATE state){
	if(state == SW_ST_OFF){
		sw_off(p_sw);
	}
	else if(state == SW_ST_ON){
		sw_on(p_sw);
	}
}

#endif /* COMPONENT_SWITCH_SWITCH_H_ */
