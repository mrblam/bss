/*
 * ioe_hw.h
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_IOE_HW_IOE_HW_H_
#define BOARD_STM32_BSP_IOE_HW_IOE_HW_H_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct IOE_hw_t IOE_hw;
struct IOE_hw_t{
	I2C_HandleTypeDef i2c_com;
};

extern IOE_hw	ioe_module;

void ioe_hw_init(void);
void ioe_hw_write(IOE_hw* p_hw, const uint8_t add, const uint8_t* data);
uint8_t ioe_hw_read(IOE_hw* p_hw,const uint8_t add);

#endif /* BOARD_STM32_BSP_IOE_HW_IOE_HW_H_ */
