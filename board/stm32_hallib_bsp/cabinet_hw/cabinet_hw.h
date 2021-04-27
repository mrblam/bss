/*
 * cabinet_hw.h
 *
 *  Created on: Apr 22, 2021
 *      Author: nguyenquang
 */

#ifndef BOARD_STM32_HALLIB_BSP_CABINET_HW_CABINET_HW_H_
#define BOARD_STM32_HALLIB_BSP_CABINET_HW_CABINET_HW_H_

#include "stm32f1xx_hal.h"
#include "gpio_hw.h"
#include "interrupt_hw.h"
#include "app_config.h"

#define IO_SCAN_TIMER				TIM3

#define HW_SW_ON                    GPIO_PIN_SET
#define HW_SW_OFF                   GPIO_PIN_RESET

#define NODE_ID1_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_1,GPIO_PIN_SET)
#define NODE_ID2_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_2,GPIO_PIN_SET)
#define NODE_ID3_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_3,GPIO_PIN_SET)
#define NODE_ID4_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_4,GPIO_PIN_SET)
#define NODE_ID5_HIGH               HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_5,GPIO_PIN_SET)
#define NODE_ID6_HIGH               HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_6,GPIO_PIN_SET)
#define NODE_ID7_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_7,GPIO_PIN_SET)
#define NODE_ID8_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_8,GPIO_PIN_SET)
#define NODE_ID9_HIGH               HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_9,GPIO_PIN_SET)
#define NODE_ID10_HIGH              HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_10,GPIO_PIN_SET)
#define NODE_ID11_HIGH              HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_11,GPIO_PIN_SET)
#define NODE_ID12_HIGH              HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_12,GPIO_PIN_SET)
#define NODE_ID13_HIGH              HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_13,GPIO_PIN_SET)
#define NODE_ID14_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_14,GPIO_PIN_SET)
#define NODE_ID15_HIGH				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_15,GPIO_PIN_SET)

#define NODE_ID1_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_1,GPIO_PIN_RESET)
#define NODE_ID2_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_2,GPIO_PIN_RESET)
#define NODE_ID3_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_3,GPIO_PIN_RESET)
#define NODE_ID4_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_4,GPIO_PIN_RESET)
#define NODE_ID5_LOW               	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_5,GPIO_PIN_RESET)
#define NODE_ID6_LOW               	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_6,GPIO_PIN_RESET)
#define NODE_ID7_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_7,GPIO_PIN_RESET)
#define NODE_ID8_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_8,GPIO_PIN_RESET)
#define NODE_ID9_LOW               	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_9,GPIO_PIN_RESET)
#define NODE_ID10_LOW              	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_10,GPIO_PIN_RESET)
#define NODE_ID11_LOW              	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_11,GPIO_PIN_RESET)
#define NODE_ID12_LOW              	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_12,GPIO_PIN_RESET)
#define NODE_ID13_LOW              	HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_13,GPIO_PIN_RESET)
#define NODE_ID14_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_14,GPIO_PIN_RESET)
#define NODE_ID15_LOW				HAL_GPIO_WritePin(CELL_NODE_ID_PORT,CELL_NODE_ID_15,GPIO_PIN_RESET)

#define DOOR_GET_STATE(id)			(door_state&(1<<(id)))
extern uint32_t door_state;
extern uint8_t cab_temp[CABINET_CELL_NUM];
extern TIM_HandleTypeDef io_scan_timer;

/*
typedef void (*door_hw_act)(uint8_t id);
door_hw_act door_sw_on;
*/
void cabinet_hw_init(void);

void door_sw_on(uint16_t id);

#endif /* BOARD_STM32_HALLIB_BSP_CABINET_HW_CABINET_HW_H_ */
