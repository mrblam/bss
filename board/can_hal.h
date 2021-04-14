/*
 * can_hal.h
 *
 *  Created on: Aug 20, 2020
 *      Author: quangnd
 */

#ifndef BOARD_CAN_HAL_H_
#define BOARD_CAN_HAL_H_
#include "can_hardware.h"
#include "compiler_optimize.h"

void can_hardware_init(void) WEAK;
#endif /* BOARD_CAN_HAL_H_ */
