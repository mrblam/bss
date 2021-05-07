/*
 * debug_io_hw.h
 *
 *  Created on: Oct 9, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_DEBUG_IO_HW_DEBUG_IO_HW_H_
#define BOARD_STM32_BSP_DEBUG_IO_HW_DEBUG_IO_HW_H_

#define IO1_SET_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define IO1_SET_LOW GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define IO2_SET_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define IO2_SET_LOW GPIO_ResetBits(GPIOA,GPIO_Pin_12)

void debug_io_hw_init(void);

#endif /* BOARD_STM32_BSP_DEBUG_IO_HW_DEBUG_IO_HW_H_ */
