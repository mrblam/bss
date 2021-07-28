/*
 * master_hw.c
 *
 *  Created on: Jul 28, 2021
 *      Author: KhanhDinh
 */

#include "master_hw.h"

static void master_led_init(void);

void master_hw_init(void){
	master_led_init();
}

void master_led_init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_WritePin(MASTER_LED_PORT, MASTER_LED1_PIN|MASTER_LED2_PIN|MASTER_LED3_PIN|MASTER_LED4_PIN, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MASTER_LED1_PIN|MASTER_LED2_PIN|MASTER_LED3_PIN|MASTER_LED4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MASTER_LED_PORT, &GPIO_InitStruct);
}
