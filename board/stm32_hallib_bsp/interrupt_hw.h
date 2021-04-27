/*
 * interrupt_hw.h
 *
 *  Created on: Aug 22, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_INTERRUPT_HW_H_
#define BOARD_STM32_BSP_INTERRUPT_HW_H_

#include "timer_hw.h"
#include "stm32f1xx_it.h"

/* System Tick IRQ */
#define STATE_MACHINE_UPDATE_TICK 	SysTick_Handler
#define DISABLE_STM_UPDATE_TICK	 		SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |\
                   SysTick_CTRL_TICKINT_Msk   |\
                   SysTick_CTRL_ENABLE_Msk

#define ENABLE_STM_UPDATE_TICK SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk |\
                   SysTick_CTRL_TICKINT_Msk   |\
                   SysTick_CTRL_ENABLE_Msk)

/* HMI_MSG_process IRQ */
#define HMI_PROCESS_DATA_IRQ			TIM2_IRQHandler
#define CHECK_TIM_IRQ_REQUEST			HAL_TIM_IRQHandler

/* HMI Send Command IRQ */
#define GET_HMI_COMMAND_IRQ				USART1_IRQHandler
#define CHECK_COM_IRQ_REQUEST			HAL_UART_IRQHandler

/* IRQ_Priority */
#define GET_HMI_CMD_IRQ_PRIORITY		0
#define MSG_PROCESS_IRQ_PRIORITY		1
#define CAN_IRQN_PRIORITY		        2
#define ADC_IRQN_PRIORITY				3
#define IO_SCAN_TIM_PRIORITY			4

#endif /* BOARD_STM32_BSP_INTERRUPT_HW_H_ */
