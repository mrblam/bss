/*
 * adc.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Admin
 */

#include "stm32f10x.h"
#include "adc.h"
#include "../GPIO_DoorSt/GPIO.h"

void Config_Adc_Ntc()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	config_gpio_(GPIOA,GPIO_PinSource5,GPIO_Mode_AIN,GPIO_Speed_50MHz);
	ADC_InitTypeDef ADC_STRUCT;
	ADC_STRUCT.ADC_ContinuousConvMode=ENABLE;
	ADC_STRUCT.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_STRUCT.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_STRUCT.ADC_Mode=ADC_Mode_Independent;
	ADC_STRUCT.ADC_NbrOfChannel=1;
	ADC_STRUCT.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_STRUCT);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_13Cycles5);
	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
uint16_t Value_Adc_Ntc()
{
	uint16_t value_adc_ntc=0;
	for(uint8_t i=0;i<5;i++)
	{
		if(i==0) value_adc_ntc=ADC_GetConversionValue(ADC1);
		value_adc_ntc=(value_adc_ntc+ADC_GetCalibrationStatus(ADC1))/2;
	}
	return value_adc_ntc;
}
