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

typedef struct ADC_hw_t ADC_hw;

void adc_hw_init(void) WEAK;

#endif /* BOARD_ADC_HW_HAL_H_ */
