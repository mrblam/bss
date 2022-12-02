/*
 * tim_hw.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#include "timer_hw.h"

TIM_HandleTypeDef hmi_timer;

static void hmi_process_data_timer_init(void);
static void hmi_process_data_timer_nvic(void);

void timer_hw_init(void){
	hmi_process_data_timer_init();
	hmi_process_data_timer_nvic();
	HAL_TIM_Base_Start_IT(&hmi_timer);
}

/* Config IRQ_TIMER2 per 1ms to process HMI messages */
static void hmi_process_data_timer_init(void){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	hmi_timer.Instance = TIMER2;
	hmi_timer.Init.Prescaler = 71;
	hmi_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	hmi_timer.Init.Period = 999;
	hmi_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hmi_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&hmi_timer) != HAL_OK){
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&hmi_timer, &sClockSourceConfig) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&hmi_timer, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	HAL_TIM_Base_Start_IT(&hmi_timer);
}

static void hmi_process_data_timer_nvic(void){
	/* TIM2 interrupt Init */
	HAL_NVIC_SetPriority(TIM2_IRQn, TIMER2_IRQN_PRIORITY, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
#if 1
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle){
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_TIM3_CLK_ENABLE();
}
#endif
