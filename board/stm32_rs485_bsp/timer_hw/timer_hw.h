/*
 * tim_hw.h
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef BOARD_STM32_HALLIB_BSP_TIMER_HW_TIMER_HW_H_
#define BOARD_STM32_HALLIB_BSP_TIMER_HW_TIMER_HW_H_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"
#include "cabinet_hw.h"

extern TIM_HandleTypeDef hmi_timer;

void timer_hw_init(void);

#define TIMER2		TIM2

#endif /* BOARD_STM32_HALLIB_BSP_TIMER_HW_TIMER_HW_H_ */
