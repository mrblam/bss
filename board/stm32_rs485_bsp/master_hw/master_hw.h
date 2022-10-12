/*
 * master_hw.h
 *
 *  Created on: Jul 28, 2021
 *      Author: KhanhDinh
 */

#ifndef BOARD_STM32_RS485_BSP_MASTER_HW_MASTER_HW_H_
#define BOARD_STM32_RS485_BSP_MASTER_HW_MASTER_HW_H_

#include "compiler_optimize.h"
#include "stm32f1xx_hal.h"


/*CTRL_LED*/
#define BSS_LED_PORT				GPIOB
#define BSS_CTRL_LED_RED			GPIO_PIN_13
#define BSS_CTRL_LED1				GPIO_PIN_14
#define BSS_CTRL_LED2				GPIO_PIN_15
#define BSS_CTRL_TOP_LED			GPIO_PIN_12

#define BSS_LED_ENABLE				HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED1 | BSS_CTRL_LED2 | BSS_CTRL_TOP_LED , GPIO_PIN_SET);
#define BSS_DISABLE_LED				HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED1 | BSS_CTRL_LED2 | BSS_CTRL_TOP_LED , GPIO_PIN_RESET);

#define BSS_CTRL_LED_RED_HIGH		HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED_RED, GPIO_PIN_SET)
#define BSS_CTRL_LED_RED_LOW		HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED_RED, GPIO_PIN_RESET)
#define BSS_CTRL_LED1_HIGH			HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED1, GPIO_PIN_SET)
#define BSS_CTRL_LED2_HIGH			HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED2, GPIO_PIN_SET)
#define BSS_CTRL_TOP_LED_HIGH		HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_TOP_LED, GPIO_PIN_SET)

#define BSS_CTRL_LED1_LOW			HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED1, GPIO_PIN_RESET)
#define BSS_CTRL_LED2_LOW			HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED2, GPIO_PIN_RESET)
#define BSS_CTRL_TOP_LED_LOW		HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_TOP_LED, GPIO_PIN_RESET)

/* CTRL_FAN */
#define BSS_FAN_PORT				GPIOB
#define BSS_CTRL_FAN1				GPIO_PIN_5
#define BSS_CTRL_FAN2				GPIO_PIN_4

#define BSS_CTRL_FAN1_HIGH			HAL_GPIO_WritePin(BSS_FAN_PORT, BSS_CTRL_FAN1, GPIO_PIN_SET)
#define BSS_CTRL_FAN2_HIGH			HAL_GPIO_WritePin(BSS_FAN_PORT, BSS_CTRL_FAN2, GPIO_PIN_SET)

#define BSS_CTRL_FAN1_LOW			HAL_GPIO_WritePin(BSS_FAN_PORT, BSS_CTRL_FAN1, GPIO_PIN_RESET)
#define BSS_CTRL_FAN2_LOW			HAL_GPIO_WritePin(BSS_FAN_PORT, BSS_CTRL_FAN2, GPIO_PIN_RESET)

 /* CTRL_SPEAKER */
#define BSS_CTRL_SPEAKER_PORT		GPIOB
#define BSS_CTRL_SPEAKER_PIN 		GPIO_PIN_1

#define BSS_CTRL_SPEAKER_HIGH		HAL_GPIO_WritePin(BSS_CTRL_SPEAKER_PORT,BSS_CTRL_SPEAKER_PIN,GPIO_PIN_SET)
#define BSS_CTRL_SPEAKER_LOW		HAL_GPIO_WritePin(BSS_CTRL_SPEAKER_PORT,BSS_CTRL_SPEAKER_PIN,GPIO_PIN_RESET)

/*CTRL_CHARGER */
#define BSS_CTRL_CHARGER1_PORT		GPIOB
#define BSS_CTRL_CHARGER1_PIN		GPIO_PIN_3
#define BSS_CTRL_CHARGER1_HIGH		HAL_GPIO_WritePin(BSS_CTRL_CHARGER1_PORT,BSS_CTRL_CHARGER1_PIN,GPIO_PIN_SET)
#define BSS_CTRL_CHARGER1_LOW		HAL_GPIO_WritePin(BSS_CTRL_CHARGER1_PORT,BSS_CTRL_CHARGER1_PIN,GPIO_PIN_RESET)

#define BSS_CTRL_CHARGER2_PORT		GPIOA
#define BSS_CTRL_CHARGER2_PIN		GPIO_PIN_15
#define BSS_CTRL_CHARGER2_HIGH		HAL_GPIO_WritePin(BSS_CTRL_CHARGER2_PORT,BSS_CTRL_CHARGER2_PIN,GPIO_PIN_SET)
#define BSS_CTRL_CHARGER2_LOW		HAL_GPIO_WritePin(BSS_CTRL_CHARGER2_PORT,BSS_CTRL_CHARGER2_PIN,GPIO_PIN_RESET)

/*BATTERY 12,4V BACKUP*/
#define CHG_ENB_PORT				GPIOB
#define CHG_ENB_PIN 				GPIO_PIN_9
#define CHG_ENB_HIGH				HAL_GPIO_WritePin(CHG_ENB_PORT,CHG_ENB_PIN,GPIO_PIN_SET)
#define CHG_ENB_LOW					HAL_GPIO_WritePin(CHG_ENB_PORT,CHG_ENB_PIN,GPIO_PIN_RESET)

#define RD_VOLTAGE_BS_PORT			GPIOA
#define RD_VOLTAGE_BS_PIN			GPIO_PIN_4


void master_hw_init(void);

#endif /* BOARD_STM32_RS485_BSP_MASTER_HW_MASTER_HW_H_ */
