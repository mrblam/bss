/*
 * PCF8575.c
 *
 *  Created on: Jan 25, 2021
 *      Author: Admin
 */
#include "stm32f10x.h"
#include "../I2C/I2C.h"
#include "PCF8575.h"

extern uint8_t DIGITAL_FAN[16];
void ON_OFF_FAN(uint8_t slave_adress,uint8_t FANx,uint8_t ON_OFF)
{
	if(ON_OFF)  DIGITAL_FAN[FANx-1]=1;
	else DIGITAL_FAN[FANx-1]=0;
	uint8_t control_fan_1_8=0;
	uint8_t control_fan_9_16=0;
	for(int i=0;i<8;i++)
	{
		if(DIGITAL_FAN[i]!=0) {control_fan_1_8+=(uint8_t)pow(2,i);}
		if(DIGITAL_FAN[i+8]!=0) {control_fan_9_16+=(uint8_t)pow(2,i);}
	}
	SEND_CONTROL_PCF8575(slave_adress,control_fan_1_8,control_fan_9_16);
}


