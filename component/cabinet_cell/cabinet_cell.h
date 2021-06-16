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

typedef enum CABINET_OP_STATE_t{
	CAB_CELL_ST_INACTIVE		= 0,
	CAB_CELL_ST_EMPTY 			= 1,
	CAB_CELL_ST_BP_ID_ASSIGN	= 2,
	CAB_CELL_ST_BP_ID_AUTHORIZE	= 3,
	CAB_CELL_ST_STANDBY 		= 4,
	CAB_CELL_ST_CHARGING 	= 5,
	CAB_CELL_ST_INIT
} CABINET_OP_STATE;

struct Cabinet_t{
	CABINET_OP_STATE 	op_state;
	uint8_t				cab_id;
	BP*					bp;
	Cabinet_Door		door;
	Switch				cell_fan;
	Switch				charger;
	Switch				node_id_sw;
	uint8_t				temp;
	uint8_t 			is_changed;
	void (*data_serialize)(Cabinet* p_cab, char* buff);
	void (*on_door_close)(Cabinet* p_cab);
	void (*on_door_open)(Cabinet* p_cab);
};

void cab_cell_init(Cabinet* p_cab);
void cab_cell_reset(Cabinet* p_cab);
CABINET_OP_STATE cab_cell_get_state(Cabinet* p_cab);
void cab_cell_update_state(Cabinet* p_cab);
void cab_cell_update_door_state(Cabinet* p_cab, DOOR_STATE new_state);
void cab_cell_update_fan_state(Cabinet* p_cab, SW_STATE new_state);
void cab_cell_update_temp(Cabinet* p_cab, uint8_t new_temp);
void cab_cell_open_door(Cabinet* p_cab);
void cab_cell_update_bp_data(Cabinet* p_cab, int32_t* p_data, int32_t new_var);
void cab_cell_update_bp_array_data(Cabinet* p_cab, int32_t p_data[], uint8_t arr_size, int32_t new_var[]);

static inline void cab_cell_data_serialize(Cabinet* p_cab, char* buff){
	p_cab->data_serialize(p_cab, buff);
}

static inline void cab_cell_set_op_state(Cabinet* p_cab, CABINET_OP_STATE op_state){
	p_cab->op_state = op_state;
}

#endif /* COMPONENT_CABINET_CELL_CABINET_CELL_H_ */
