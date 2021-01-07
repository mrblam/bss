/*
 * adc_hw_hal.h
 *
 *  Created on: Dec 21, 2020
 *      Author: quangnd
 */

#ifndef BOARD_ADC_HW_HAL_H_
#define BOARD_ADC_HW_HAL_H_

#include "stdint.h"
#include "compiler_optimize.h"
#include "adc_hw.h"

typedef struct ADC_Hw_t ADC_Hw;

void adc_hw_init(void) WEAK;
uint32_t adc_get_ntc1_value(void) WEAK;
uint32_t adc_get_ntc2_value(void) WEAK;
uint32_t bp_vol_get_adc(const uint8_t chan) WEAK;

#endif /* BOARD_ADC_HW_HAL_H_ */
