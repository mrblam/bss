/*
 * hc05_hw.c
 *
 *  Created on: Oct 5, 2020
 *      Author: quangnd
 */

#include "neo_6m_hw.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_usart.h"
#include "stdlib.h"
#include "interrupt_hw.h"
#include "string.h"
#include "hw_delay_hal.h"
#include "uart_hw.h"

/* this define is for ioc v2.1 uart1 */
#define USART_DEV_CLK	                        RCC_APB2Periph_USART1
#define USART_PORT_CLK	                        RCC_AHBPeriph_GPIOA
#define USART_PORT		                GPIOA
#define USART_AF	                        GPIO_AF_1
#define USART_TX_PORT	                        GPIOA
#define USART_TX_PIN	                        GPIO_Pin_9
#define USART_TX_PINSOURCE	                GPIO_PinSource9
#define USART_RX_PORT	                        GPIOA
#define USART_RX_PIN	                        GPIO_Pin_10
#define USART_RX_PINSOURCE	                GPIO_PinSource10
#define USART_MODULE                            USART1
#define USART_INTERRUPT_VECTOR                  USART1_IRQn
#define NEO6M_USART_ISR                         USART2_IRQHandler
#define RCC_INIT                                RCC_APB2PeriphClockCmd(USART_DEV_CLK, ENABLE); \
                        RCC_AHBPeriphClockCmd(USART_PORT_CLK, ENABLE)

static void usart_clk_init(void);
static void usart_gpio_init(void);
static void usart_module_init(void);
static void usart_nvic_init(void);

static void neo6m_send(const char ch);
static void neo6m_sends(const char* s);
static void neo6m_send_len(const char* s,const int32_t len);

Uart_Hw neo6m_hw;

void neo6m_hw_init(void){
	usart_clk_init();
	usart_gpio_init();
	usart_module_init();
	usart_nvic_init();

	neo6m_hw.send=neo6m_send;
	neo6m_hw.sends=neo6m_sends;
	neo6m_hw.send_len=neo6m_send_len;
}

static void neo6m_send(const char ch){
	while(USART_GetFlagStatus(USART_MODULE,USART_FLAG_TXE) == RESET);
	USART_SendData(USART_MODULE,(uint16_t)ch);
}

static void neo6m_sends(const char* s){
	while(*s){
		while(USART_GetFlagStatus(USART_MODULE,USART_FLAG_TXE) == RESET);
		USART_SendData(USART_MODULE,*s);
		s++;
	}
}

static void neo6m_send_len(const char* s,const int32_t len){

	for(int i=0;i<len;i++){
		while(USART_GetFlagStatus(USART_MODULE,USART_FLAG_TXE) == RESET);
		USART_SendData(USART_MODULE,s[i]);
	}
}

static void usart_clk_init(void){
        RCC_INIT;
}

static void usart_gpio_init(void){

	GPIO_InitTypeDef gpio_init_structure;
	/*Init usart1 gpio PB7 - RX, PB6 - TX, AF0 */
	GPIO_PinAFConfig(USART_PORT, USART_TX_PINSOURCE, USART_AF);
	GPIO_PinAFConfig(USART_PORT, USART_RX_PINSOURCE, USART_AF);
	gpio_init_structure.GPIO_Pin = USART_TX_PIN | USART_RX_PIN;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_structure.GPIO_OType = GPIO_OType_PP;
	gpio_init_structure.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT, &gpio_init_structure);
}
static void usart_module_init(void)
{
	USART_InitTypeDef usart_init_structure;
	/* Baud rate 9600, 8-bit data, One stop bit
	 * No parity, Do both Rx and Tx, No HW flow control
	 */
	usart_init_structure.USART_BaudRate = 9600;
	usart_init_structure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	usart_init_structure.USART_WordLength = USART_WordLength_8b;
	usart_init_structure.USART_StopBits = USART_StopBits_1;
	usart_init_structure.USART_Parity = USART_Parity_No;
	usart_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_OverrunDetectionConfig(USART_MODULE,USART_OVRDetection_Disable);
	/* Init usart1 + usart2 */
	USART_Init(USART_MODULE, &usart_init_structure);
	/* Enable RXNE interrupt */
	USART_ITConfig(USART_MODULE, USART_IT_RXNE, ENABLE);
	/*Enable USART	 */
	USART_Cmd(USART_MODULE, ENABLE);
}
static void usart_nvic_init(void)
{
	NVIC_InitTypeDef nvic_init_structure;
	nvic_init_structure.NVIC_IRQChannel = USART_INTERRUPT_VECTOR;
	nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
	nvic_init_structure.NVIC_IRQChannelPriority = SIM800_INTERRUPT_PRIORITY;
	NVIC_Init(&nvic_init_structure);
}

void NEO6M_USART_ISR(void)
{
	if(USART_GetITStatus(USART_MODULE, USART_IT_RXNE) != RESET)
	{
		neo6m_hw.buff[neo6m_hw.rx_index++]=USART_ReceiveData(USART_MODULE);
		if(neo6m_hw.rx_index>=UART_RX_BUFF_SIZE) neo6m_hw.rx_index=0;
	}
	if(USART_GetITStatus(USART_MODULE,USART_IT_ORE)){
	        USART_ClearFlag(USART_MODULE,USART_FLAG_ORE);
	}
}
