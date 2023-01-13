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
#include "string.h"

typedef struct Cabinet_t Cabinet;

typedef enum CABINET_OP_STATE_t{
	CAB_CELL_ST_INACTIVE		= 0,
	CAB_CELL_ST_EMPTY 			= 1,
	CAB_CELL_ST_BP_ID_ASSIGN	= 2,
	CAB_CELL_ST_BP_ID_AUTHORIZE	= 3,
	CAB_CELL_ST_STANDBY 		= 4,
	CAB_CELL_ST_CHARGING 		= 5,
	CAB_CELL_ST_INIT			= 6,
	CAB_CELL_ST_FAIL
} CABINET_OP_STATE;

typedef enum LED_COLOR_t{
	RED = 0,
	BLUE = 1,
	BLINK = 3,
	NONE = 4
} LED_COLOR;

typedef struct Cabinet_led_t Cabinet_led;
typedef void (*led_act)(Cabinet_led* p_led);
struct Cabinet_led_t{
	LED_COLOR	color;
	led_act		update_color;
};

struct Cabinet_t{
	CABINET_OP_STATE 	op_state;
	uint8_t				cab_id;
	BP*					bp;
	Cabinet_Door		door;
	Switch				cell_fan;
	Switch				charger;
	Switch				node_id_sw;
	Cabinet_led			led;
	uint8_t				temp;
	uint8_t 			is_changed;
	void (*data_serialize)(Cabinet* p_cab, char* buff);
	CABINET_OP_STATE 	fail_state;
};

void cab_cell_init(Cabinet* , uint8_t);
void cab_cell_reset(Cabinet* p_cab);
void cab_cell_reset_io(Cabinet* p_cab);
void cab_cell_update_state(Cabinet* p_cab);
void cab_cell_update_door_state(Cabinet* p_cab, DOOR_STATE new_state);
void cab_cell_open_door(Cabinet* p_cab);
void cab_cell_update_io_state(Cabinet* p_cab);
void cab_cell_update_led_state(Cabinet* p_cab);

static inline void cab_cell_data_serialize(Cabinet* p_cab, char* buff){
	p_cab->data_serialize(p_cab, buff);
}

static inline void cab_cell_set_op_state(Cabinet* p_cab, CABINET_OP_STATE op_state){
	p_cab->op_state = op_state;
}

static inline CABINET_OP_STATE cab_cell_get_op_state(Cabinet* p_cab){
	return p_cab->op_state;
}

static inline void cab_cell_set_led_color(Cabinet* p_cab, LED_COLOR color){
	p_cab->led.color = color;
	p_cab->led.update_color(&p_cab->led);
}

#endif /* COMPONENT_CABINET_CELL_CABINET_CELL_H_ */
