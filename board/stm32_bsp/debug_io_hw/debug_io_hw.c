/*
 * debug_io_hw.c
 *
 *  Created on: Oct 9, 2020
 *      Author: quangnd
 */

#include "debug_io_hw.h"
#include "stm32f0xx_rcc.h"

static void debug_io_rcc_init(void);
static void debug_io_gpio_init(void);

void debug_io_hw_init(void){
	debug_io_rcc_init();
	debug_io_gpio_init();
}

static void debug_io_rcc_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
}

static void debug_io_gpio_init(void){

	GPIO_InitTypeDef GPIO_Initstructure;

		/* OUTPUT PIN */
		GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_OUT;
		GPIO_Initstructure.GPIO_OType  = GPIO_OType_PP;
		GPIO_Initstructure.GPIO_Speed = GPIO_Speed_10MHz;

		GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_11;
		GPIO_Init(GPIOA, &GPIO_Initstructure);

		GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_12;
		GPIO_Init(GPIOA, &GPIO_Initstructure);
}
