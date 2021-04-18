/*
 * uart_hw_hal.h
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef BOARD_UART_HW_HAL_H_
#define BOARD_UART_HW_HAL_H_

#include "compiler_optimize.h"
#include "stdint.h"
#include "uart_hw.h"

void uart_hw_init(void) WEAK;

#endif /* BOARD_UART_HW_HAL_H_ */
