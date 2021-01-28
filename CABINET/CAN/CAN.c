/*
 * CAN.c
 *
 *  Created on: Jan 28, 2021
 *      Author: Admin
 */


#include "stm32f10x.h"
#include"CAN.h"
#include "../GPIO_DoorSt/GPIO.h"

GPIO GPIO_CAN_TX={GPIOA,GPIO_Pin_12};
GPIO GPIO_CAN_RX={GPIOA,GPIO_Pin_11};
void config_CAN()
{
	config_gpio_(GPIO_CAN_TX.GPIOx,GPIO_CAN_TX.GPIO_Pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
	config_gpio_(GPIO_CAN_RX.GPIOx,GPIO_CAN_RX.GPIO_Pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	CAN_InitTypeDef CAN_STRUCT;
	//CAN_STRUCT.CAN_
}

