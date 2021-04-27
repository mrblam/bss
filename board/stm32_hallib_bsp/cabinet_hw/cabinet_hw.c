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

static door_hw_act door_update_state;
uint32_t door_state;

static void cabinet_io_scan_timer_init(void);
static void cabinet_io_scan_timer_init_nvic(void);
static void cabinet_door_scan_state(void);

static void door1_sw_on(uint8_t id);
static void door2_sw_on(uint8_t id);
static void door3_sw_on(uint8_t id);
static void door4_sw_on(uint8_t id);
static void door5_sw_on(uint8_t id);
static void door6_sw_on(uint8_t id);
static void door7_sw_on(uint8_t id);
static void door8_sw_on(uint8_t id);
static void door9_sw_on(uint8_t id);
static void door10_sw_on(uint8_t id);
static void door11_sw_on(uint8_t id);
static void door12_sw_on(uint8_t id);
static void door13_sw_on(uint8_t id);
static void door14_sw_on(uint8_t id);
static void door15_sw_on(uint8_t id);

static void door1_update_state(uint8_t id);
static void door2_update_state(uint8_t id);
static void door3_update_state(uint8_t id);
static void door4_update_state(uint8_t id);
static void door5_update_state(uint8_t id);
static void door6_update_state(uint8_t id);
static void door7_update_state(uint8_t id);
static void door8_update_state(uint8_t id);
static void door9_update_state(uint8_t id);
static void door10_update_state(uint8_t id);
static void door11_update_state(uint8_t id);
static void door12_update_state(uint8_t id);
static void door13_update_state(uint8_t id);
static void door14_update_state(uint8_t id);
static void door15_update_state(uint8_t id);

static door_hw_act door_update_state_interface[] = {
		door1_update_state, door2_update_state, door3_update_state, door4_update_state, door5_update_state,
		door6_update_state, door7_update_state, door8_update_state, door9_update_state, door10_update_state,
		door11_update_state, door12_update_state, door13_update_state, door14_update_state, door15_update_state};

static door_hw_act door_sw_on_interface[] = {
		door1_sw_on, door2_sw_on, door3_sw_on, door4_sw_on, door5_sw_on,
		door6_sw_on, door7_sw_on, door8_sw_on, door9_sw_on, door10_sw_on,
		door11_sw_on, door12_sw_on,	door13_sw_on, door14_sw_on, door15_sw_on};

void cabinet_hw_init(void){
	cabinet_io_scan_timer_init();
	gpio_hw_init();
	for(uint8_t id = CAB1; id < CAB15; id++){
		door_sw_on = door_sw_on_interface[id];
		door_update_state = door_update_state_interface[id];
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

static void cabinet_door_scan_state(void){
	for(uint8_t cab_id = CAB1; cab_id < CAB15; cab_id++){
		door_update_state(cab_id);
	}
}

void TIM3_IRQHandler(void){
	cabinet_door_scan_state();
	HAL_TIM_IRQHandler(&io_scan_timer);
}

/* -------------------------------------------------------------------------------------------------- */
/* CAB 1,2,3,5,6 work */
static void door1_sw_on(uint8_t id){
	(void)id;
	uint32_t cnt = 0;
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_1, GPIO_PIN_SET);
		while(cnt < 100000) cnt++;
		cnt = 0;
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_1, GPIO_PIN_RESET);
		while(cnt < 100000) cnt++;
		cnt = 0;
	}
}

static void door2_sw_on(uint8_t id){
	(void)id;
	uint32_t cnt = 0;
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_2) == GPIO_PIN_SET){
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_2, GPIO_PIN_SET);
		while(cnt < 100000) cnt++;
		cnt = 0;
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_2, GPIO_PIN_RESET);
		while(cnt < 100000) cnt++;
		cnt = 0;
	}
}

static void door3_sw_on(uint8_t id){
	(void)id;
	uint32_t cnt = 0;
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_3) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_3, GPIO_PIN_SET);
		while(cnt < 100000) cnt++;
		cnt = 0;
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_3, GPIO_PIN_RESET);
		while(cnt < 100000) cnt++;
		cnt = 0;
	}
}

static void door4_sw_on(uint8_t id){
	(void)id;
}

static void door5_sw_on(uint8_t id){
	(void)id;
	uint32_t cnt = 0;
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_4) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_4, GPIO_PIN_SET);
		while(cnt < 100000) cnt++;
		cnt = 0;
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_4, GPIO_PIN_RESET);
		while(cnt < 100000) cnt++;
		cnt = 0;
	}
}

static void door6_sw_on(uint8_t id){
	(void)id;
	uint32_t cnt = 0;
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_5) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_5, GPIO_PIN_SET);
		while(cnt < 100000) cnt++;
		cnt = 0;
		HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_5, GPIO_PIN_RESET);
		while(cnt < 100000) cnt++;
		cnt = 0;
	}
}

static void door7_sw_on(uint8_t id){
	(void)id;
}

static void door8_sw_on(uint8_t id){
	(void)id;
}

static void door9_sw_on(uint8_t id){
	(void)id;
}

static void door10_sw_on(uint8_t id){
	(void)id;
}

static void door11_sw_on(uint8_t id){
	(void)id;
}

static void door12_sw_on(uint8_t id){
	(void)id;
}

static void door13_sw_on(uint8_t id){
	(void)id;
}

static void door14_sw_on(uint8_t id){
	(void)id;
}

static void door15_sw_on(uint8_t id){
	(void)id;
}

/* -------------------------------------------------------------------------------------------------- */
/* CAB 1,2,3,5,6 work */
static void door1_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door2_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_2);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door3_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_3);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door4_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_4);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door5_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_5);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door6_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_6);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door7_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_7);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door8_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_8);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door9_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_9);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door10_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_10);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door11_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_11);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door12_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_12);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door13_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_13);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door14_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_14);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}

static void door15_update_state(uint8_t id){
	uint8_t pre_state = (door_state & (1<<id));
	uint8_t cur_state = HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_15);
	uint16_t stable_cnt = 0;
	while(stable_cnt < 100){
		if(cur_state == pre_state){
			stable_cnt++;
		}
		else stable_cnt = 0;
	}
	door_state = ((door_state & (~(cur_state<<id))) | (cur_state<<id));
}
