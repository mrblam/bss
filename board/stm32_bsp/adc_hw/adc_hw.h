/*
 * adc_hw.h
 *
 *  Created on: Dec 21, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_ADC_HW_ADC_HW_H_
#define BOARD_STM32_BSP_ADC_HW_ADC_HW_H_
#include "stdint.h"
#include "stm32f10x.h"

struct ADC_Hw_t{
	int32_t adc_value;
	int32_t adc_offset;
};

void adc_hw_init(void);
#endif /* BOARD_STM32_BSP_ADC_HW_ADC_HW_H_ */
