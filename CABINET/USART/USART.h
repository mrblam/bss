/*
 * USART.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Admin
 */

#ifndef USART_USART_H_
#define USART_USART_H_
#include "stm32f10x.h"
#include "../GPIO_DoorSt/GPIO.h"


extern GPIO GPIO_RX_UART1;
extern GPIO GPIO_TX_UART1;
extern GPIO GPIO_RX_UART2;
extern GPIO GPIO_TX_UART2;
extern GPIO GPIO_RX_UART3;
extern GPIO GPIO_TX_UART3;

extern uint8_t data_usart2[100];
extern uint8_t flag_usart2;
extern uint8_t data_usart3[100];
extern uint8_t flag_usart3;
extern uint8_t data_usart1[100];
extern uint8_t flag_usart1;

void config_usart(USART_TypeDef *USARTx,GPIO GPIO_RX_USARTx,GPIO GPIO_TX_USARTx);
void SEND_USART_DATA(USART_TypeDef *USARTx,uint16_t *data);

#endif /* USART_USART_H_ */
