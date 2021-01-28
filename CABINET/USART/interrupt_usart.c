/*
 * interrupt_usart.c
 *
 *  Created on: Jan 26, 2021
 *      Author: Admin
 */


#include "stm32f10x.h"
uint8_t data_usart1[100];
uint8_t cout_usart1=0;
uint8_t flag_usart1=0;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
	 uint8_t data;
	 data=USART_ReceiveData(USART1);
	 if(cout_usart1==0) {for (uint8_t i=0;i<100;i++) data_usart1[i]=0;}
	 if(data==13) {cout_usart1=0;flag_usart1=1;}
	 else {data_usart1[cout_usart1]=data; cout_usart1++;}
	  if(cout_usart1==100) cout_usart1=0;

     USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

uint8_t data_usart2[100];
uint8_t cout_usart2=0;
uint8_t flag_usart2=0;
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
	 uint8_t data;
	 data=USART_ReceiveData(USART2);
	 if(cout_usart2==0) {for (uint8_t i=0;i<100;i++) data_usart1[i]=0;}
	 if(data==13) {cout_usart2=0;flag_usart2=1;}
	 else {data_usart2[cout_usart2]=data; cout_usart2++;}
	  if(cout_usart2==100) cout_usart2=0;

     USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}

uint8_t data_usart3[100];
uint8_t cout_usart3=0;
uint8_t flag_usart3=0;
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
	 uint8_t data;
	 data=USART_ReceiveData(USART3);
	 if(cout_usart3==0) {for (uint8_t i=0;i<100;i++) data_usart3[i]=0;}
	 if(data==13) {cout_usart3=0;flag_usart3=1;}
	 else {data_usart3[cout_usart3]=data; cout_usart3++;}
	  if(cout_usart3==100) cout_usart3=0;

     USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}
