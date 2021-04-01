/*
 * _74HC4067.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Admin
 */
#include "stm32f10x.h"
#include "../ADC/adc.h"
#include "../GPIO_DoorSt/GPIO.h"

GPIO S0={GPIOD,GPIO_Pin_2};
GPIO S1={GPIOA,GPIO_Pin_0};
GPIO S2={GPIOA,GPIO_Pin_1};
GPIO S3={GPIOA,GPIO_Pin_4};
BitAction BITACTION_16_Sx[16][4]={{0,0,0,0},
{1,0,0,0},
{0,1,0,0},
{1,1,0,0},
{0,0,1,0},
{1,0,1,0},
{0,1,1,0},
{1,1,1,0},
{0,0,0,1},
{1,0,0,1},
{0,1,0,1},
{1,1,0,1},
{0,0,1,1},
{1,0,1,1},
{0,1,1,1},
{1,1,1,1}
};
void config_S()
{
	config_gpio_(S0.GPIOx,S0.GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	config_gpio_(S1.GPIOx,S1.GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	config_gpio_(S2.GPIOx,S2.GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	config_gpio_(S3.GPIOx,S3.GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	Config_Adc_Ntc();
}
void redirect_ntc_channel(BitAction BitAction0,BitAction BitAction1,BitAction BitAction2,BitAction BitAction3)
{
	GPIO_WriteBit(S0.GPIOx,S0.GPIO_Pin,BitAction0);
	GPIO_WriteBit(S1.GPIOx,S1.GPIO_Pin,BitAction1);
	GPIO_WriteBit(S2.GPIOx,S2.GPIO_Pin,BitAction2);
	GPIO_WriteBit(S3.GPIOx,S3.GPIO_Pin,BitAction3);
}
uint16_t * Read_16_NTC()
{
static uint16_t value_16_ntc[16];
for(uint8_t i=0;i<16;i++)
{
	redirect_ntc_channel(BITACTION_16_Sx[i][0],BITACTION_16_Sx[i][1],BITACTION_16_Sx[i][2],BITACTION_16_Sx[i][3]);
	value_16_ntc[i]=Value_Adc_Ntc();
}

return value_16_ntc;
}


