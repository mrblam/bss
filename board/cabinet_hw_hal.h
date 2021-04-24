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

void cabinet_hw_init(void) WEAK;

#endif /* BOARD_CABINET_HW_HAL_H_ */
