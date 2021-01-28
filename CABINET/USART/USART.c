/*
 * USART.c
 *
 *  Created on: Jan 26, 2021
 *      Author: Admin
 */
#include "stm32f10x.h"
#include "../GPIO_DoorSt/GPIO.h"
#include "USART.h"
GPIO GPIO_RX_UART1={GPIOA,GPIO_Pin_10};
GPIO GPIO_TX_UART1={GPIOA,GPIO_Pin_9};
GPIO GPIO_RX_UART2={GPIOA,GPIO_Pin_3};
GPIO GPIO_TX_UART2={GPIOA,GPIO_Pin_2};
GPIO GPIO_RX_UART3={GPIOB,GPIO_Pin_11};
GPIO GPIO_TX_UART3={GPIOB,GPIO_Pin_10};
void config_usart(USART_TypeDef *USARTx,GPIO GPIO_RX_USARTx,GPIO GPIO_TX_USARTx)
{
 config_gpio_(GPIO_RX_USARTx.GPIOx,GPIO_RX_USARTx.GPIO_Pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
 config_gpio_(GPIO_TX_USARTx.GPIOx,GPIO_TX_USARTx.GPIO_Pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

 USART_InitTypeDef USART_STRUCT;
 USART_STRUCT.USART_BaudRate=9600;
 USART_STRUCT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
 USART_STRUCT.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
 USART_STRUCT.USART_Parity=USART_Parity_No;
 USART_STRUCT.USART_StopBits=USART_StopBits_1;
 USART_STRUCT.USART_WordLength=USART_WordLength_8b;
 USART_Init(USARTx,&USART_STRUCT);
    USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
 	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
 	USART_Cmd(USARTx,ENABLE);

 	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
 	NVIC_InitTypeDef NVIC_STRUCT;
 	NVIC_STRUCT.NVIC_IRQChannelCmd=ENABLE;
 	if(USARTx==USART1){
 		NVIC_STRUCT.NVIC_IRQChannel=USART1_IRQn;
       	NVIC_STRUCT.NVIC_IRQChannelPreemptionPriority=1;
 	    NVIC_STRUCT.NVIC_IRQChannelSubPriority=1;
 	}
 	else if(USARTx==USART2){
 		NVIC_STRUCT.NVIC_IRQChannel=USART2_IRQn;
       	NVIC_STRUCT.NVIC_IRQChannelPreemptionPriority=2;
 	    NVIC_STRUCT.NVIC_IRQChannelSubPriority=2;
 	}
 	else if(USARTx==USART3){
 		NVIC_STRUCT.NVIC_IRQChannel=USART3_IRQn;
       	NVIC_STRUCT.NVIC_IRQChannelPreemptionPriority=3;
 	    NVIC_STRUCT.NVIC_IRQChannelSubPriority=3;
 	}
 	NVIC_Init(&NVIC_STRUCT);
}

void SEND_USART_DATA(USART_TypeDef *USARTx,uint16_t *data)
{
  for(uint8_t i=0;i<sizeof(data);i++)
   {
	  while (USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
	  USART_SendData(USARTx,*(data+i));
    }
}



