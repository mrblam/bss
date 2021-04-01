/*
 * GPIO.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Admin
 */

#include "GPIO.h"
#include "stm32f10x.h"

        GPIO DoorST1={GPIOA,GPIO_Pin_6};
		GPIO DoorST2={GPIOA,GPIO_Pin_7};
		GPIO DoorST3={GPIOA,GPIO_Pin_8};
		GPIO DoorST4={GPIOA,GPIO_Pin_15};
		GPIO DoorST5={GPIOB,GPIO_Pin_0};
		GPIO DoorST6={GPIOB,GPIO_Pin_1};
		GPIO DoorST7={GPIOB,GPIO_Pin_2};
		GPIO DoorST8={GPIOB,GPIO_Pin_3};
		GPIO DoorST9={GPIOB,GPIO_Pin_4};
		GPIO DoorST10={GPIOB,GPIO_Pin_5};
		GPIO DoorST11={GPIOB,GPIO_Pin_8};
		GPIO DoorST12={GPIOB,GPIO_Pin_9};
		GPIO DoorST13={GPIOB,GPIO_Pin_12};
		GPIO DoorST14={GPIOB,GPIO_Pin_13};
		GPIO DoorST15={GPIOB,GPIO_Pin_14};
		GPIO DoorST16={GPIOB,GPIO_Pin_15};
void config_gpio_(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
{
	uint32_t RCC_APB2_GPIOx;
	if(GPIOx==GPIOA) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOA;
	 else if(GPIOx==GPIOB) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOB;
	   else if(GPIOx==GPIOC) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOC;
	    else if(GPIOx==GPIOD) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOD;
	     else if(GPIOx==GPIOE) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOE;
	       else if(GPIOx==GPIOF) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOF;
	         else if(GPIOx==GPIOG) RCC_APB2_GPIOx=RCC_APB2Periph_GPIOG;
	          else return;
	RCC_APB2PeriphClockCmd(RCC_APB2_GPIOx,ENABLE);
	GPIO_InitTypeDef GPIO_STRUCT;
	GPIO_STRUCT.GPIO_Mode=GPIO_Mode;
	GPIO_STRUCT.GPIO_Pin=GPIO_Pin;
	GPIO_STRUCT.GPIO_Speed=GPIO_Speed;
	GPIO_Init(GPIOx,&GPIO_STRUCT);


}

void config_16_Gpio_Door_St()
{

	GPIO* doorst[16]={&DoorST1,&DoorST2,&DoorST3,&DoorST4,&DoorST5,&DoorST6,&DoorST7,&DoorST8,&DoorST9,&DoorST10,&DoorST11,&DoorST12,&DoorST13,&DoorST14,&DoorST15,&DoorST16};
    for(uint8_t i=0;i<16;i++)
    {
    	config_gpio_(doorst[i]->GPIOx,doorst[i]->GPIO_Pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
    }
}

uint8_t READ_Doorst(GPIO DoorSTx)
{
 return GPIO_ReadInputDataBit(DoorSTx.GPIOx,DoorSTx.GPIO_Pin);

}




