/*
 * core_hw.c
 *
 *  Created on: Sep 16, 2020
 *      Author: quangnd
 */

#include "stm32f10x.h"
#include "core_hw.h"

void core_hw_init(void){
	SystemInit();
	SystemCoreClockUpdate();
	//SysTick_Config(SystemCoreClock/SYSTICK_FREQ_Hz);
	SysTick_Config(SystemCoreClock/1000);
}


