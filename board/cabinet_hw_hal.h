/*
 * cabinet_hw_hal.h
 *
 *  Created on: Apr 22, 2021
 *      Author: nguyenquang
 */

#ifndef BOARD_CABINET_HW_HAL_H_
#define BOARD_CABINET_HW_HAL_H_
#include "compiler_optimize.h"
#include "cabinet_hw.h"

#define FAN_ON                          HW_SW_ON
#define FAN_OFF                         HW_SW_OFF

#define HAL_CAB1_FAN_SW(sw)                             CAB1_FAN_SW(sw)

#define HAL_NODE_ID1_HIGH								NODE_ID1_HIGH
#define HAL_NODE_ID2_HIGH								NODE_ID2_HIGH
#define HAL_NODE_ID3_HIGH								NODE_ID3_HIGH
#define HAL_NODE_ID4_HIGH								NODE_ID4_HIGH
#define HAL_NODE_ID5_HIGH								NODE_ID5_HIGH
#define HAL_NODE_ID6_HIGH								NODE_ID6_HIGH
#define HAL_NODE_ID7_HIGH								NODE_ID7_HIGH
#define HAL_NODE_ID8_HIGH								NODE_ID8_HIGH
#define HAL_NODE_ID9_HIGH								NODE_ID9_HIGH
#define HAL_NODE_ID10_HIGH								NODE_ID10_HIGH
#define HAL_NODE_ID11_HIGH								NODE_ID11_HIGH
#define HAL_NODE_ID12_HIGH								NODE_ID12_HIGH
#define HAL_NODE_ID13_HIGH								NODE_ID13_HIGH
#define HAL_NODE_ID14_HIGH								NODE_ID14_HIGH
#define HAL_NODE_ID15_HIGH								NODE_ID15_HIGH

#define HAL_NODE_ID1_LOW								NODE_ID1_LOW
#define HAL_NODE_ID2_LOW								NODE_ID2_LOW
#define HAL_NODE_ID3_LOW								NODE_ID3_LOW
#define HAL_NODE_ID4_LOW								NODE_ID4_LOW
#define HAL_NODE_ID5_LOW								NODE_ID5_LOW
#define HAL_NODE_ID6_LOW								NODE_ID6_LOW
#define HAL_NODE_ID7_LOW								NODE_ID7_LOW
#define HAL_NODE_ID8_LOW								NODE_ID8_LOW
#define HAL_NODE_ID9_LOW								NODE_ID9_LOW
#define HAL_NODE_ID10_LOW								NODE_ID10_LOW
#define HAL_NODE_ID11_LOW								NODE_ID11_LOW
#define HAL_NODE_ID12_LOW								NODE_ID12_LOW
#define HAL_NODE_ID13_LOW								NODE_ID13_LOW
#define HAL_NODE_ID14_LOW								NODE_ID14_LOW
#define HAL_NODE_ID15_LOW								NODE_ID15_LOW

#define HAL_CELL_FAN_1_HIGH								CELL_FAN_1_HIGH
#define HAL_CELL_FAN_2_HIGH								CELL_FAN_1_HIGH
#define HAL_CELL_FAN_3_HIGH								CELL_FAN_1_HIGH
#define HAL_CELL_FAN_4_HIGH								CELL_FAN_1_HIGH
#define HAL_CELL_FAN_5_HIGH								CELL_FAN_1_HIGH

#define HAL_CELL_FAN_1_LOW								CELL_FAN_1_LOW
#define HAL_CELL_FAN_2_LOW								CELL_FAN_1_LOW
#define HAL_CELL_FAN_3_LOW								CELL_FAN_1_LOW
#define HAL_CELL_FAN_4_LOW								CELL_FAN_1_LOW
#define HAL_CELL_FAN_5_LOW								CELL_FAN_1_LOW

#define HAL_DOOR_GET_STATE(id)		door_state[id]

void door_sw_on(uint16_t id) WEAK;
void cabinet_hw_init(void) WEAK;

#endif /* BOARD_CABINET_HW_HAL_H_ */
