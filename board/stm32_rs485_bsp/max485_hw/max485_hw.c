/*
 * max485_hw.c
 *
 *  Created on: May 7, 2021
 *      Author: quangnd
 */

#include "max485_hw.h"

struct Max485_Hw_t cabinet_485_hw;
static void max485_send(struct Max485_Hw_t* p_hw, const char c);
static void max485_sends(struct Max485_Hw_t* p_hw, const char* s);
static void max485_gpio_hw_init(void);

void max485_hw_init(void){
	max485_gpio_hw_init();
	cabinet_485_hw.send=max485_send;
	cabinet_485_hw.sends=max485_sends;
}

static void max485_gpio_hw_init(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(EN_RS485_PORT, EN_RS485_PIN, GPIO_PIN_RESET);

	  /*Configure GPIO pin : PtPin */
	  GPIO_InitStruct.Pin = EN_RS485_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(EN_RS485_PORT, &GPIO_InitStruct);
}

void max485_hw_set_baud(struct Max485_Hw_t* p_hw,const uint32_t baud){
	(void)p_hw;
	(void)baud;
}

static void max485_sends(struct Max485_Hw_t* p_hw, const char* s){
	while(*s){
		HAL_UART_Transmit(&p_hw->uart_module,(uint8_t*) s, 1, 500);
		s++;
	}
}

static void max485_send(struct Max485_Hw_t* p_hw, const char c){
		HAL_UART_Transmit(&p_hw->uart_module,(uint8_t*) &c, 1, 500);
}

