/*
 * max485_hw.c
 *
 *  Created on: May 7, 2021
 *      Author: quangnd
 */

#include "max485_hw.h"

#define MAX485_UART_PORT	USART2
#define UART_BAUDRATE		115200

struct Max485_Hw_t cabinet_485_hw;
static void max485_send(struct Max485_Hw_t* p_hw, const char c);
static void max485_sends(struct Max485_Hw_t* p_hw, const char* s);

void max485_hw_init(void){

	cabinet_485_hw.send=max485_send;
	cabinet_485_hw.sends=max485_sends;
}

void max485_hw_set_baud(struct Max485_Hw_t* p_hw,const uint32_t baud){

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

