/*
 * test_sync_data.c
 *
 *  Created on: Apr 16, 2021
 *      Author: KhanhDinh
 */

#include "stm32f1xx_hal.h"

static void uart_init(void);
UART_HandleTypeDef huart1;

int main(void){
	uart_init();
	HAL_UART_Transmit(&huart1, (uint8_t*)"hello", sizeof("hello"), 1000);
}

static void uart_init(void){

	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = 115200;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  if(huart1.Instance==USART1){
	    __HAL_RCC_USART1_CLK_ENABLE();
	    __HAL_RCC_GPIOA_CLK_ENABLE();

	    GPIO_InitStruct.Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin = GPIO_PIN_10;
	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(USART1_IRQn);
	  }


}
