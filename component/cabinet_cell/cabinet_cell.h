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
#include "app_config.h"
#include "cabinet_door.h"
#include "switch.h"
#include "ntc.h"
#include "bat_pack.h"

typedef struct Cabinet_cell_t Cabinet_cell;

typedef enum CABIN_CELL_STATE{
	CELL_ST_EMPTY 		= 0,
	CELL_ST_STANDBY 	= 1,
	CELL_ST_CHARGING 	= 2,
	CELL_ST_MAINTAIN 	= 3,
	CELL_ST_FAULT 		= 4
} CABIN_CELL_STATE;

struct Cabinet_cell_t{
	CABIN_CELL_STATE 	state;
	uint8_t				cab_id;
	uint8_t				node_id;
	BP*					bp;
	Cabinet_door*		door;
	Switch*				cell_fan;
	NTC*				temp_ss;
	int32_t				temp;
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

#endif /* COMPONENT_CABINET_CELL_CABINET_CELL_H_ */
