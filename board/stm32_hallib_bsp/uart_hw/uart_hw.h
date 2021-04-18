#ifndef BOARD_STM32_HALLIB_BSP_UART_HW_UART_HW_H_
#define BOARD_STM32_HALLIB_BSP_UART_HW_UART_HW_H_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"

#define UART_SIM	USART2
#define UART_HMI	USART3
#define UART_POWER	USART1

typedef struct UART_HW_t UART_HW;

extern UART_HandleTypeDef uart_sim;
extern UART_HandleTypeDef uart_power;
extern UART_HandleTypeDef uart_hmi;

void uart_hw_init(void);
void uart_sends(UART_HW* p_hw,const uint8_t* s);

void debug_com_hw_init(void);
//void debug_port_set_receive_handle(Uart_Receive_Handle handle);
//void USART_Write_String(char *a);

#endif
