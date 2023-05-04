#ifndef BOARD_STM32_HALLIB_BSP_UART_HW_UART_HW_H_
#define BOARD_STM32_HALLIB_BSP_UART_HW_UART_HW_H_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"
#include "interrupt_hw.h"

typedef struct UART_hw_t UART_hw;
struct UART_hw_t{
	UART_HandleTypeDef 	uart_module;
	DMA_HandleTypeDef	uart_DMA;
	uint8_t				rx_data;

	void				(*receive_handle)(UART_hw* p_hw);
	void				(*uart_send)(UART_hw* p_hw);
};

extern UART_hw hmi_com;
extern UART_hw debug_com;
extern UART_hw rs485_com;


#define RS485_PORT_COM			USART1
#define DEBUG_PORT_COM			USART2
#define HMI_PORT_COM			USART3

#define UART_BAUDRATE_HMI		115200
#define UART_BAUDRATE_SLAVE		115200
#define UART_BAUDRATE_DEBUG		9600

void uart_hw_init(void);
void uart_sends(UART_hw* p_hw, uint8_t* s);
char uart_receives(UART_hw* p_hw, char* s);
void UART_DMA_Init(void);
void UART_set_baudrate_rs485(int baudrate);
#endif
