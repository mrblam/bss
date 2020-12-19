/*
 * ioe_hw.h
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_IOE_HW_IOE_HW_H_
#define BOARD_STM32_BSP_IOE_HW_IOE_HW_H_

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stdint.h"


typedef struct IOE_Hw_t IOE_Hw;

struct IOE_Hw_t{
        I2C_TypeDef* i2c_module;
        uint8_t address;
};

void ioe_hw_init(void);
void ioe_hw_write_reg(IOE_Hw* p_hw,const uint8_t add,const uint8_t data);
uint8_t ioe_hw_read_reg(IOE_Hw* p_hw,const uint8_t add);

#endif /* BOARD_STM32_BSP_IOE_HW_IOE_HW_H_ */
