/*
 * BSS_hw.c
 *
 *  Created on: Jul 28, 2021
 *      Author: KhanhDinh
 */

#include "master_hw.h"


static void bss_led_init(void);
static void bss_fan_init(void);
static void bss_speaker_init(void);
static void bss_ctrl_charger_init(void);
static void	bss_backup_power_init(void);


void master_hw_init(void){
	bss_led_init();
	bss_fan_init();
	bss_speaker_init();
	bss_ctrl_charger_init();
	bss_backup_power_init();
}

void bss_led_init(void)
{
	/*led 2 side init*/
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_LED_RED | BSS_CTRL_LED_GREEN | BSS_CTRL_LED_BLUE , GPIO_PIN_RESET);
		GPIO_InitStruct.Pin = BSS_CTRL_LED_RED | BSS_CTRL_LED_GREEN | BSS_CTRL_LED_BLUE;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BSS_LED_PORT, &GPIO_InitStruct);

	/*top led init*/
		//GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(BSS_LED_PORT, BSS_CTRL_TOP_LED , GPIO_PIN_RESET);
		GPIO_InitStruct.Pin = BSS_CTRL_TOP_LED;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BSS_LED_PORT, &GPIO_InitStruct);
		/**/

}
void bss_fan_init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(BSS_FAN1_PORT, BSS_CTRL_FAN1, GPIO_PIN_RESET);
		/*Configure GPIO pins : PB4 PB5 */
		GPIO_InitStruct.Pin = BSS_CTRL_FAN1;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BSS_FAN1_PORT, &GPIO_InitStruct);
		HAL_GPIO_Init(BSS_FAN2_PORT, &GPIO_InitStruct);
}
void bss_speaker_init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(BSS_CTRL_SPEAKER_PORT, BSS_CTRL_SPEAKER_PIN, GPIO_PIN_RESET);
		/*Configure GPIO pins : PB1 */
		GPIO_InitStruct.Pin = BSS_CTRL_SPEAKER_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BSS_CTRL_SPEAKER_PORT, &GPIO_InitStruct);
	}
void bss_ctrl_charger_init()
{
	/*CTRL CHARGER 1*/
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(BSS_CTRL_CHARGER1_PORT, BSS_CTRL_CHARGER1_PIN, GPIO_PIN_RESET);
		/*Configure GPIO pins : PB3 */
		GPIO_InitStruct.Pin = BSS_CTRL_CHARGER1_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BSS_CTRL_CHARGER1_PORT, &GPIO_InitStruct);

	/*CTRL CHARGER 2 */
		//GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(BSS_CTRL_CHARGER2_PORT, BSS_CTRL_CHARGER2_PIN, GPIO_PIN_RESET);
		/*Configure GPIO pins : PA15 */
		GPIO_InitStruct.Pin = BSS_CTRL_CHARGER2_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BSS_CTRL_CHARGER2_PORT, &GPIO_InitStruct);

}
void bss_backup_power_init(void)
{
	/*	AC line ShutDown
	 * EN BACKUP PIN
	 * RD Voltage BS
	 * CHG ENB
	 * */
}
