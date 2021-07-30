/*
 * BSS_hw.c
 *
 *  Created on: Jul 28, 2021
 *      Author: KhanhDinh
 */

#include "master_hw.h"

static void bss_led_init(void);

void master_hw_init(void){
	bss_led_init();
}

void bss_led_init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_WritePin(BSS_LED_PORT, BSS_LED_CTRL_PIN|BSS_LED_RED_PIN|BSS_LED_BLUE_PIN|BSS_LED_GREEN_PIN, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = BSS_LED_CTRL_PIN|BSS_LED_RED_PIN|BSS_LED_BLUE_PIN|BSS_LED_GREEN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BSS_LED_PORT, &GPIO_InitStruct);
}
