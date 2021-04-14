/*
 * adc_hw.c
 *
 *  Created on: Dec 21, 2020
 *      Author: quangnd
 */
#include "adc_hw.h"

#define ARRAYSIZE 4
#define ADC1_DR    ((uint32_t)0x4001244C)

static volatile uint16_t ADC_values[ARRAYSIZE];

#define NTC1_ADC_VALUE_INDEX                            0
#define NTC2_ADC_VALUE_INDEX                            1
#define BACKUP_BAT_VOL_ADC_VALUE_INDEX                  2
#define BP_VOL_ADC_VALUE_INDEX                          3

static void adc_channels_init(void);
static void adc_dma_init(void);

void adc_hw_init(void){
}

static void adc_channels_init(void){
}

static void adc_dma_init(void){
}
