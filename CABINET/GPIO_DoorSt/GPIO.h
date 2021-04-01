/*
 * GPIO.h
 *
 *  Created on: Jan 24, 2021
 *      Author: Admin
 */

#ifndef GPIO_DOORST_GPIO_H_
#define GPIO_DOORST_GPIO_H_
#include "stm32f10x.h"
typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}GPIO;
extern GPIO DoorST1;
extern GPIO DoorST2;
extern GPIO DoorST3;
extern GPIO DoorST4;
extern GPIO DoorST5;
extern GPIO DoorST6;
extern GPIO DoorST7;
extern GPIO DoorST8;
extern GPIO DoorST9;
extern GPIO DoorST10;
extern GPIO DoorST11;
extern GPIO DoorST12;
extern GPIO DoorST13;
extern GPIO DoorST14;
extern GPIO DoorST15;
extern GPIO DoorST116;
void config_gpio_(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);
void config_16_Gpio_Door_St();
uint8_t READ_Doorst(GPIO DoorSTx);
#endif /* GPIO_DOORST_GPIO_H_ */
