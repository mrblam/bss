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

typedef struct IOE_Hw_t IOE_Hw;
struct IOE_Hw_t{
	I2C_HandleTypeDef 	i2c_com;
	void				(*ioe_write)(IOE_Hw* p_hw, const uint8_t addr, uint8_t* data);
};

extern IOE_Hw	ioe_module;

void ioe_hw_init(void);

#endif /* BOARD_STM32_BSP_IOE_HW_IOE_HW_H_ */
