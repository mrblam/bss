/*
 * ioe_hw.h
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_IOE_HW_IOE_HW_H_
#define BOARD_STM32_BSP_IOE_HW_IOE_HW_H_

#include "stdint.h"


typedef struct IOE_Hw_t IOE_Hw;

struct IOE_Hw_t{
        uint8_t address;
};

void ioe_hw_init(void);
void ioe_hw_write_reg(IOE_Hw* p_hw,const uint8_t add,const uint8_t data);
uint8_t ioe_hw_read_reg(IOE_Hw* p_hw,const uint8_t add);

#endif /* BOARD_STM32_BSP_IOE_HW_IOE_HW_H_ */
