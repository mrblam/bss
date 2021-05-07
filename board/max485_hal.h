/*
 * max485_hal.h
 *
 *  Created on: May 7, 2021
 *      Author: quangnd
 */

#ifndef BOARD_MAX485_HAL_H_
#define BOARD_MAX485_HAL_H_
#include "max485_hw.h"
#include "compiler_optimize.h"

typedef struct Max485_Hw_t Max485_Hw;

#define HAL_MAX485_SET_DIR_TX						MAX485_SET_DIR_TX
#define HAL_MAX485_SET_DIR_RX						MAX485_SET_DIR_RX

void max485_hw_init(void) WEAK;
void max485_hw_set_baud(Max485_Hw* p_hw,const uint32_t baud) WEAK;

#endif /* BOARD_MAX485_HAL_H_ */
