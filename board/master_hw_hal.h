/*
 * master_hw_hal.h
 *
 *  Created on: Jul 28, 2021
 *      Author: KhanhDinh
 */

#ifndef BOARD_MASTER_HW_HAL_H_
#define BOARD_MASTER_HW_HAL_H_

#include "master_hw.h"

#define HAL_BSS_LED_ENABLE 							BSS_LED_ENABLE
#define HAL_BSS_LED_RED_SET_HIGH 					BSS_LED_RED_SET_HIGH
#define HAL_BSS_LED_BLUE_SET_HIGH 					BSS_LED_BLUE_SET_HIGH
#define HAL_BSS_LED_GREEN_SET_HIGH 					BSS_LED_GREEN_SET_HIGH
#define HAL_BSS_LED_DISABLE 						BSS_LED_DISABLE
#define HAL_BSS_LED_RED_SET_LOW 					BSS_LED_RED_SET_LOW
#define HAL_BSS_LED_BLUE_SET_LOW 					BSS_LED_BLUE_SET_LOW
#define HAL_BSS_LED_GREEN_SET_LOW	 				BSS_LED_GREEN_SET_LOW

void master_hw_init(void) WEAK;

#endif /* BOARD_MASTER_HW_HAL_H_ */
