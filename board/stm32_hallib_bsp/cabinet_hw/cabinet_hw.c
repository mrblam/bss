/*
 * cabinet_hw.c
 *
 *  Created on: Apr 22, 2021
 *      Author: nguyenquang
 */
#include "cabinet_hw.h"
#include "gpio_hw.h"
#include "app_config.h"

TIM_HandleTypeDef io_scan_timer;

uint32_t door_state;
static void cabinet_io_scan_timer_init(void);
static void cabinet_io_scan_timer_init_nvic(void);
static void cabinet_door_scan_state(void);
static void cabinet_io_scan_timer_init_nvic(void);
static void door_update_state(uint16_t id);

#define UPDATE_DOOR_STATE(id)	door_state &= ~(1<<(id)); door_state|= (cur_state<<(id))

static GPIO_TypeDef* door_state_ports[]={
		DOOR_ST_PORT1_4,
		DOOR_ST_PORT1_4,
		DOOR_ST_PORT1_4,
		DOOR_ST_PORT1_4,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16,
		DOOR_ST_PORT5_16
};

static uint16_t door_state_pins[]={
		DOOR_ST_1,
		DOOR_ST_2,
		DOOR_ST_3,
		DOOR_ST_4,
		DOOR_ST_5,
		DOOR_ST_6,
		DOOR_ST_7,
		DOOR_ST_8,
		DOOR_ST_9,
		DOOR_ST_10,
		DOOR_ST_11,
		DOOR_ST_12,
		DOOR_ST_13,
		DOOR_ST_14,
		DOOR_ST_15
};

static GPIO_TypeDef* door_sw_ports[] = {
		DOOR_SW_PORT,
		DOOR_SW_PORT,
		DOOR_SW_PORT,
		DOOR_SW_PORT,
		DOOR_SW_PORT,
};

static uint16_t door_sw_pins[] = {
		DOOR_SW_1,
		DOOR_SW_2,
		DOOR_SW_3,
		DOOR_SW_4,
		DOOR_SW_5
};

void cabinet_hw_init(void){
	cabinet_io_scan_timer_init();
	cabinet_io_scan_timer_init_nvic();
	gpio_hw_init();
}

void TIM3_IRQHandler(void){
	cabinet_door_scan_state();
	HAL_TIM_IRQHandler(&io_scan_timer);
}

static void cabinet_door_scan_state(void){
	for(uint16_t i=0;i<15;i++){
		door_update_state(i);
	}
}

static void door_update_state(uint16_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(door_state_ports[id], door_state_pins[id]);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 1000){
		pre_state = cur_state;
		cur_state = HAL_GPIO_ReadPin(door_state_ports[id], door_state_pins[id]);
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	UPDATE_DOOR_STATE(id);
}

void door_sw_on(uint16_t id){
	uint32_t cnt = 0;
	while(HAL_GPIO_ReadPin(door_state_ports[id], door_state_pins[id]) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(door_sw_ports[id],door_sw_pins[id], GPIO_PIN_SET);
		while(cnt < 100000) cnt++;
		cnt = 0;
		HAL_GPIO_WritePin(door_sw_ports[id],door_sw_pins[id], GPIO_PIN_RESET);
		while(cnt < 100000) cnt++;
		cnt = 0;
	}
}

/* -------------------------------------------------------------------------------------------------- */
/* Interrupt every 1ms */
static void cabinet_io_scan_timer_init(void){
	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  io_scan_timer.Instance = IO_SCAN_TIMER;
	  io_scan_timer.Init.Prescaler = 71;
	  io_scan_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	  io_scan_timer.Init.Period = 999;
	  io_scan_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  io_scan_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&io_scan_timer) != HAL_OK){
		  Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&io_scan_timer, &sClockSourceConfig) != HAL_OK){
		  Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&io_scan_timer, &sMasterConfig) != HAL_OK){
		  Error_Handler();
	  }

	  HAL_TIM_Base_Start_IT(&io_scan_timer);
}

static void cabinet_io_scan_timer_init_nvic(void){
	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM3_IRQn, IO_SCAN_TIM_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}



