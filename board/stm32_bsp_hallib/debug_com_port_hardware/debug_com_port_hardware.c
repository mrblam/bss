#include "debug_com_port_hardware.h"

#include "stdio.h"
#include "stdlib.h"

UART_HW power_sync_port;
UART_HW	hmi_sync_port;



void uart_hw_init(void){
	power_sync_hw_gpio_init();
	hmi_sync_hw_gpio_init();
}

void power_sync_hw_gpio_init(void){
	GPIO_InitTypeDef PowerSync_GPIO_InitStruct;

	PowerSync_GPIO_InitStruct.GPIO_Pin = POWER_SYNC_PORT_TX_PIN;
	PowerSync_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	PowerSync_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PowerSync_GPIO_InitStruct);

	PowerSync_GPIO_InitStruct.GPIO_Pin = POWER_SYNC_PORT_RX_PIN;
	PowerSync_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	PowerSync_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PowerSync_GPIO_InitStruct);
}

void hmi_sync_hw_gpio_init(void){
	GPIO_InitTypeDef HMISync_GPIO_InitStruct;

	HMISync_GPIO_InitStruct.GPIO_Pin = HMI_SYNC_PORT_TX_PIN;
	HMISync_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	HMISync_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &HMISync_GPIO_InitStruct);

	HMISync_GPIO_InitStruct.GPIO_Pin = HMI_SYNC_PORT_RX_PIN;
	HMISync_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	HMISync_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &HMISync_GPIO_InitStruct);
}

void uart_sends(UART_HW* p_hw,const uint8_t* s){

        (void)p_hw;
    while(*s){
    	      s++;
    }
}
