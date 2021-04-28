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
#include "cabinet_door.h"
#include "switch.h"
#include "ntc.h"
#include "string_util.h"

typedef struct Cabinet_t Cabinet;

typedef enum CABINET_STATE{
	CAB_CELL_ST_INACTIVE		= 0,
	CAB_CELL_ST_EMPTY 			= 1,
	CAB_CELL_ST_BP_ID_ASSIGN	= 2,
	CAB_CELL_ST_BP_ID_AUTHORIZE	= 3,
	CAB_CELL_ST_ST_STANDBY 		= 4,
	CAB_CELL_ST_ST_CHARGING 	= 5
} CABINET_STATE;

struct Cabinet_t{
	CABINET_STATE 	state;
	uint8_t			cab_id;
	uint8_t			node_id;
	BP*				bp;
	Cabinet_Door		door;
	Switch			cell_fan;
	Switch			charger;
	Switch			node_id_sw;
	NTC*			temp_ss;
	int32_t			temp;
	void (*data_serialize)(Cabinet* p_cc, char* buff);
	void (*on_door_close)(Cabinet* p_cab);
	void (*on_door_open)(Cabinet* p_cab);
};

void cabinet_init(Cabinet* p_cc);
void cab_cell_set_state(Cabinet* p_cc);
CABINET_STATE cab_cell_get_state(Cabinet* p_cc);
void cab_cell_update_state(Cabinet* p_cab);
void cab_cell_update_door_state(Cabinet* p_cc);
void cab_cell_open_door(Cabinet* p_cc);
int32_t cab_cell_get_cell_temp(Cabinet* p_cc);
void cab_cell_fan_turn_on(Cabinet* p_cc);
void cab_cell_fan_turn_off(Cabinet* p_cc);
void cab_cell_deactive_charger(Cabinet* p_cc);
void cab_cell_active_charger(Cabinet* p_cc);
void cab_cell_active_cell_fan(Cabinet* p_cc);
void cab_cell_deactive_cell_fan(Cabinet* p_cc);

static inline void cab_cell_data_serialize(Cabinet* p_cc, char* buff){
	p_cc->data_serialize(p_cc, buff);
}

#endif /* COMPONENT_CABINET_CELL_CABINET_CELL_H_ */
