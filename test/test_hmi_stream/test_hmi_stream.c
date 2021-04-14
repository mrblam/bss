/*
 * test_hmi_stream.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_it.h"

int main(void){
	board_init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_Initstructure);

	__enable_irq();

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 0);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}
