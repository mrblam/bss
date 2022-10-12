/*
 * adc_hw.h
 *
 *  Created on: Dec 21, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_ADC_HW_ADC_HW_H_
#define BOARD_STM32_BSP_ADC_HW_ADC_HW_H_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"
#include "interrupt_hw.h"

#define ADC_PORT		ADC1
#define ADC_CHANNEL		ADC_CHANNEL_4

typedef struct ADC_hw_t ADC_hw;
struct ADC_hw_t{
	ADC_HandleTypeDef	adc_module;
	int32_t 			adc_value;
	int32_t 			adc_offset;
	void				(*get_adc_handle)(ADC_hw* p_adc);
};

extern ADC_hw	ntc;
extern ADC_hw	pin_vol;

void adc_hw_init(void);

#endif /* BOARD_STM32_BSP_ADC_HW_ADC_HW_H_ */
