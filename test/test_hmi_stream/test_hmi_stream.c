/*
 * test_hmi_stream.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "delay.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal.h"

int main(void){
	board_init();

	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  //__HAL_RCC_GPIOC_CLK_ENABLE();
	  //__HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	  /*Configure GPIO pin : PC13 */
	  GPIO_InitStruct.Pin = GPIO_PIN_13;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
}

void SysTick_Handler(void){

}

