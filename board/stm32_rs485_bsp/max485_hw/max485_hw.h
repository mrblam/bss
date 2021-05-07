/*
 * max485_hw.h
 *
 *  Created on: May 7, 2021
 *      Author: quangnd
 */

#ifndef BOARD_STM32_RS485_BSP_MAX485_HW_MAX485_HW_H_
#define BOARD_STM32_RS485_BSP_MAX485_HW_MAX485_HW_H_
#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"


#define MAX485_SET_DIR_TX						HAL_GPIO_WritePin(GPIOA,GPIO_Pin_0,1)
#define MAX485_SET_DIR_RX						HAL_GPIO_WritePin(GPIOA,GPIO_Pin_0,0)


struct Max485_Hw_t{
	UART_HandleTypeDef 	uart_module;
	void				(*receive_handle)(struct Max485_Hw_t* p_hw);
	void				(*send)(struct Max485_Hw_t* p_hw,const char c);
	void				(*sends)(struct Max485_Hw_t* p_hw,const char* c);
};

extern struct Max485_Hw_t cabinet_485_hw;

void max485_hw_init(void);
void max485_hw_set_baud(struct Max485_Hw_t* p_hw,const uint32_t baud);

#endif /* BOARD_STM32_RS485_BSP_MAX485_HW_MAX485_HW_H_ */
