/*
 * adc_hw.c
 *
 *  Created on: Dec 21, 2020
 *      Author: quangnd
 */
#include "adc_hw.h"

ADC_hw	ntc;
ADC_hw	pin_vol;
uint8_t cab_id = CAB1;

static void get_adc_handle_impl(ADC_hw* p_hw);
static void adc_hw_init_module(void);
static void adc_hw_init_nvic(void);

void adc_hw_init(void){
	adc_hw_init_module();
	adc_hw_init_nvic();
	pin_vol.adc_offset = -168;
	pin_vol.get_adc_handle = get_adc_handle_impl;
	HAL_ADC_Start_IT(&pin_vol.adc_module);
}

static void adc_hw_init_module(void){
	  ADC_ChannelConfTypeDef sConfig = {0};

	  /* Common config */
	  pin_vol.adc_module.Instance = ADC_PORT;
	  pin_vol.adc_module.Init.ScanConvMode = ADC_SCAN_DISABLE;
	  pin_vol.adc_module.Init.ContinuousConvMode = DISABLE;
	  pin_vol.adc_module.Init.DiscontinuousConvMode = DISABLE;
	  pin_vol.adc_module.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  pin_vol.adc_module.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  pin_vol.adc_module.Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(&pin_vol.adc_module) != HAL_OK){
	    Error_Handler();
	  }
	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&pin_vol.adc_module, &sConfig) != HAL_OK){
	    Error_Handler();
	  }
}

static void adc_hw_init_nvic(void){
    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC_IRQN_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC_PORT)
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA5     ------> ADC1_IN4 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

static void get_adc_handle_impl(ADC_hw* p_hw){
	p_hw->adc_value = HAL_ADC_GetValue(&p_hw->adc_module);
}

void ADC1_2_IRQHandler(void){
	if (pin_vol.get_adc_handle != NULL){
		pin_vol.get_adc_handle(&pin_vol);
	}
	HAL_ADC_IRQHandler(&pin_vol.adc_module);
}

