/*
 * ioe_hal.h
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */

#ifndef BOARD_IOE_HAL_H_
#define BOARD_IOE_HAL_H_
#include "compiler_optimize.h"
#include "ioe_hw.h"

void ioe_hw_init(void) WEAK;
void ioe_hw_write(IOE_Hw* p_hw,const uint8_t add,const uint8_t* data) WEAK;
uint8_t ioe_hw_read(IOE_Hw* p_hw,const uint8_t add) WEAK;

#endif /* BOARD_IOE_HAL_H_ */
