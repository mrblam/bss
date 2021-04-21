/*
 * cabinet_cell.h
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_CABINET_CELL_CABINET_CELL_H_
#define COMPONENT_CABINET_CELL_CABINET_CELL_H_

#include "stdlib.h"
#include "stdint.h"

#include "bp_data.h"
#include "app_config.h"
#include "cabinet_door.h"
#include "switch.h"
#include "ntc.h"
#include "string_util.h"

typedef struct Cabinet_cell_t Cabinet_cell;

typedef enum CABIN_CELL_STATE{
	CAB_CELL_ST_EMPTY 		= 0,
	CAB_CELL_ST_STANDBY 	= 1,
	CAB_CELL_ST_CHARGING 	= 2,
	CAB_CELL_ST_MAINTAIN 	= 3,
	CAB_CELL_ST_FAULT 		= 4
} CABIN_CELL_STATE;

typedef enum CABIN_CELL_OP_STATE{
	CAB_CELL_OP_ST_INACTIVE,
	CAB_CELL_OP_ST_ACTIVE,
	CAB_CELL_OP_ST_FAIL
} CABIN_CELL_OP_STATE;

struct Cabinet_cell_t{
	CABIN_CELL_STATE 	state;
	CABIN_CELL_OP_STATE	op_state;
	uint8_t				cab_id;
	uint8_t				node_id;
	BP*					bp;
	Cabinet_door*		door;
	Switch*				cell_fan;
	Switch*				charger;
	NTC*				temp_ss;
	int32_t				temp;
	void				(*data_serialize)(Cabinet_cell* p_cc, char* buff);
};

Cabinet_cell* cab_cell_construct(void);

void cab_cell_set_state(Cabinet_cell* p_cc);
CABIN_CELL_STATE cab_cell_get_state(Cabinet_cell* p_cc);

uint8_t cab_cell_get_slave_id(Cabinet_cell* p_cc);
void cab_cell_open_door(Cabinet_cell* p_cc);
void cab_cell_check_door_state(Cabinet_cell* p_cc);
int32_t cab_cell_get_cell_temp(Cabinet_cell* p_cc);
void cab_cell_active_cell_fan(Cabinet_cell* p_cc);
void cab_cell_deactive_cell_fan(Cabinet_cell* p_cc);

static inline void cab_cell_data_serialize(Cabinet_cell* p_cc, char* buff){
	p_cc->data_serialize(p_cc, buff);
}

#endif /* COMPONENT_CABINET_CELL_CABINET_CELL_H_ */
