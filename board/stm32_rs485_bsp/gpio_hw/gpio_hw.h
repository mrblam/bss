/*
 * gpio.h
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef BOARD_STM32_HALLIB_BSP_GPIO_HW_GPIO_HW_H_
#define BOARD_STM32_HALLIB_BSP_GPIO_HW_GPIO_HW_H_

#include "stdlib.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "app_config.h"

/* Node ID Pins */
#define CELL_NODE_ID_PORT		GPIOC
#define CELL_NODE_ID_1			GPIO_PIN_0
#define CELL_NODE_ID_2			GPIO_PIN_1
#define CELL_NODE_ID_3			GPIO_PIN_2
#define CELL_NODE_ID_4			GPIO_PIN_3
#define CELL_NODE_ID_5			GPIO_PIN_4
#define CELL_NODE_ID_6			GPIO_PIN_5
#define CELL_NODE_ID_7			GPIO_PIN_6
#define CELL_NODE_ID_8			GPIO_PIN_7
#define CELL_NODE_ID_9			GPIO_PIN_8
#define CELL_NODE_ID_10			GPIO_PIN_9
#define CELL_NODE_ID_11			GPIO_PIN_10
#define CELL_NODE_ID_12			GPIO_PIN_11
#define CELL_NODE_ID_13			GPIO_PIN_12
#define CELL_NODE_ID_14			GPIO_PIN_13
#define CELL_NODE_ID_15			GPIO_PIN_14
#define CELL_NODE_ID_16			GPIO_PIN_15

/* Door Switch Pins */
#define DOOR_SW_PORT			CELL_NODE_ID_PORT
#define DOOR_SW_1				CELL_NODE_ID_6
#define DOOR_SW_2				CELL_NODE_ID_7
#define DOOR_SW_3				CELL_NODE_ID_8
#define DOOR_SW_4				CELL_NODE_ID_9
#define DOOR_SW_5				CELL_NODE_ID_10

/* Cell Fan Switch Pins */
#define CELL_FAN_SW_PORT		CELL_NODE_ID_PORT
#define CELL_FAN_SW_1			CELL_NODE_ID_11
#define CELL_FAN_SW_2			CELL_NODE_ID_12
#define CELL_FAN_SW_3			CELL_NODE_ID_13
#define CELL_FAN_SW_4			CELL_NODE_ID_14
#define CELL_FAN_SW_5			CELL_NODE_ID_15

/* MUX Pins */
#define MUX_PORT_S0				GPIOD
#define MUX_PORT_S123			GPIOA
#define MUX_S0					GPIO_PIN_2
#define MUX_S1					GPIO_PIN_0
#define MUX_S2					GPIO_PIN_1
#define MUX_S3					GPIO_PIN_4

/* Door ST Pins */
#define DOOR_ST_PORT1_4			GPIOA
#define DOOR_ST_PORT5_16		GPIOB
#define DOOR_ST_1				GPIO_PIN_6
#define DOOR_ST_2				GPIO_PIN_7
#define DOOR_ST_3				GPIO_PIN_8
#define DOOR_ST_4				GPIO_PIN_15
#define DOOR_ST_5				GPIO_PIN_0
#define DOOR_ST_6				GPIO_PIN_1
#define DOOR_ST_7				GPIO_PIN_2
#define DOOR_ST_8				GPIO_PIN_3
#define DOOR_ST_9				GPIO_PIN_4
#define DOOR_ST_10				GPIO_PIN_5
#define DOOR_ST_11				GPIO_PIN_8
#define DOOR_ST_12				GPIO_PIN_9
#define DOOR_ST_13				GPIO_PIN_12
#define DOOR_ST_14				GPIO_PIN_13
#define DOOR_ST_15				GPIO_PIN_14
#define DOOR_ST_16				GPIO_PIN_15

void gpio_hw_init(void);



#endif /* BOARD_STM32_HALLIB_BSP_GPIO_HW_GPIO_HW_H_ */
