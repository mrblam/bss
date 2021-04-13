/*
 * test_hmi_stream.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "stm32f10x_gpio.h"
#include "stm32f1xx_hal_rcc.h"

int main(void){
	board_init();

	 __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIOC->BSRR = (uint32_t)GPIO_Pin_13 << 16;
}

