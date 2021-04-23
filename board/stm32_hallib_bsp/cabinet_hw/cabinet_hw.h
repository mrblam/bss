/*
 * cabinet_hw.h
 *
 *  Created on: Apr 22, 2021
 *      Author: nguyenquang
 */

#ifndef BOARD_STM32_HALLIB_BSP_CABINET_HW_CABINET_HW_H_
#define BOARD_STM32_HALLIB_BSP_CABINET_HW_CABINET_HW_H_

#include "stm32f1xx_hal.h"

#define HW_SW_ON                           GPIO_PIN_SET
#define HW_SW_OFF                          GPIO_PIN_RESET

#define CAB1_FAN_SW(sw)             HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,sw);

void cabinet_hw_init(void);


#endif /* BOARD_STM32_HALLIB_BSP_CABINET_HW_CABINET_HW_H_ */
