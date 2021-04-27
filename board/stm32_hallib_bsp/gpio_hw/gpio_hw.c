/*
 * gpio_hw.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#include "gpio_hw.h"

mux_act			mux_sw_channel;

static void gpio_node_id_pin_init(void);
static void gpio_mux_pin_init(void);
static void gpio_mux_pin_config(void);
static void gpio_door_st_pin_init(void);

static void mux_sw_channel0(uint8_t id);
static void mux_sw_channel1(uint8_t id);
static void mux_sw_channel2(uint8_t id);
static void mux_sw_channel3(uint8_t id);
static void mux_sw_channel4(uint8_t id);
static void mux_sw_channel5(uint8_t id);
static void mux_sw_channel6(uint8_t id);
static void mux_sw_channel7(uint8_t id);
static void mux_sw_channel8(uint8_t id);
static void mux_sw_channel9(uint8_t id);
static void mux_sw_channel10(uint8_t id);
static void mux_sw_channel11(uint8_t id);
static void mux_sw_channel12(uint8_t id);
static void mux_sw_channel13(uint8_t id);
static void mux_sw_channel14(uint8_t id);

static mux_act mux_sw_channel_interface[] = {
		mux_sw_channel7, mux_sw_channel6, mux_sw_channel5, mux_sw_channel4, mux_sw_channel3,
		mux_sw_channel2, mux_sw_channel1, mux_sw_channel0, mux_sw_channel14, mux_sw_channel13,
		mux_sw_channel12, mux_sw_channel11, mux_sw_channel10, mux_sw_channel9, mux_sw_channel8};

void gpio_hw_init(void){
	gpio_node_id_pin_init();
	gpio_mux_pin_init();
	gpio_door_st_pin_init();
}

static void gpio_node_id_pin_init(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_13|CELL_NODE_ID_14|CELL_NODE_ID_15|CELL_NODE_ID_16
	                          |CELL_NODE_ID_1|CELL_NODE_ID_2|CELL_NODE_ID_3|CELL_NODE_ID_4
	                          |CELL_NODE_ID_5|CELL_NODE_ID_6|CELL_NODE_ID_7|CELL_NODE_ID_8
	                          |CELL_NODE_ID_9|CELL_NODE_ID_10|CELL_NODE_ID_11|CELL_NODE_ID_12, GPIO_PIN_RESET);

	  /*Configure GPIO pins */
	  GPIO_InitStruct.Pin = CELL_NODE_ID_13|CELL_NODE_ID_14|CELL_NODE_ID_15|CELL_NODE_ID_16
			  	  	  	  	  |CELL_NODE_ID_1|CELL_NODE_ID_2|CELL_NODE_ID_3|CELL_NODE_ID_4
							  |CELL_NODE_ID_5|CELL_NODE_ID_6|CELL_NODE_ID_7|CELL_NODE_ID_8
							  |CELL_NODE_ID_9|CELL_NODE_ID_10|CELL_NODE_ID_11|CELL_NODE_ID_12;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void gpio_mux_pin_init(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1|MUX_S2|MUX_S3, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);

	  /*Configure GPIO pins : PA0 PA1 PA4 */
	  GPIO_InitStruct.Pin = MUX_S1|MUX_S2|MUX_S3;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(MUX_PORT_S123, &GPIO_InitStruct);

	  /*Configure GPIO pin : PD2 */
	  GPIO_InitStruct.Pin = MUX_S0;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(MUX_PORT_S0, &GPIO_InitStruct);
}

static void gpio_mux_pin_config(void){
	for(uint8_t id = CAB1; id < CAB15; id++){
		mux_sw_channel = mux_sw_channel_interface[id];
	}
}

static void gpio_door_st_pin_init(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  /*Configure GPIO pins : PA6 PA7 PA8 PA15 */
	  GPIO_InitStruct.Pin = DOOR_ST_1|DOOR_ST_2|DOOR_ST_3|DOOR_ST_4;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(DOOR_ST_PORT1_4, &GPIO_InitStruct);

	  /*Configure GPIO pins : PB0 PB1 PB2 PB12
	                           PB13 PB14 PB15 PB3
	                           PB4 PB5 PB8 PB9 */
	  GPIO_InitStruct.Pin = DOOR_ST_5|DOOR_ST_6|DOOR_ST_7|DOOR_ST_8
	                          |DOOR_ST_9|DOOR_ST_10|DOOR_ST_11|DOOR_ST_12
	                          |DOOR_ST_13|DOOR_ST_14|DOOR_ST_15|DOOR_ST_16;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(DOOR_ST_PORT5_16, &GPIO_InitStruct);
}

/* -------------------------------------------------------------------------- */

static void mux_sw_channel0(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel1(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel2(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel3(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel4(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel5(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel6(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel7(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_RESET);
}

static void mux_sw_channel8(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

static void mux_sw_channel9(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

static void mux_sw_channel10(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

static void mux_sw_channel11(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

static void mux_sw_channel12(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

static void mux_sw_channel13(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

static void mux_sw_channel14(uint8_t id){
	(void)id;
	HAL_GPIO_WritePin(MUX_PORT_S0, MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, MUX_S3, GPIO_PIN_SET);
}

