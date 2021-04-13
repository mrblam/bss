/*
 * cabinet_door.h
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_CABINET_DOOR_CABINET_DOOR_H_
#define COMPONENT_CABINET_DOOR_CABINET_DOOR_H_

#include "stdlib.h"
#include "io_state.h"
#include "switch.h"

typedef enum DOOR_STATE{
	DOOR_ST_WORK,
	DOOR_ST_FAIL
} DOOR_STATE;

typedef struct Cabinet_door_t Cabinet_door;
struct Cabinet_door_t{
	DOOR_STATE	state;
	IO_State* 	io_state;
	Switch*		solenoid;
};

Cabinet_door* door_construct(void);

static inline void cab_door_open(Cabinet_door* p_door){
	p_door->solenoid->sw_on(p_door->solenoid);
}

static inline IO_STATE cab_door_get_door_state(Cabinet_door* p_door){
	return p_door->io_state->io_get_state(p_door->io_state);
}

static inline DOOR_STATE cab_door_get_working_state(Cabinet_door* p_door){
	return p_door->state;
}

#endif /* COMPONENT_CABINET_DOOR_CABINET_DOOR_H_ */
