#ifndef BOARD_STM32_HALLIB_BSP_UART_HW_UART_HW_H_
#define BOARD_STM32_HALLIB_BSP_UART_HW_UART_HW_H_

#include "stdint.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct UART_hw_t UART_hw;
struct UART_hw_t{
	UART_HandleTypeDef 	uart_module;
	uint8_t				rx_data;
	void				(*receive_handle)(UART_hw* p_hw);
	void				(*uart_send)(UART_hw* p_hw);
};

extern UART_hw sim_port;
extern UART_hw hmi_port;
extern UART_hw power_sys_port;

#define SIM_PORT_COM		USART2
#define HMI_PORT_COM		USART3
#define POWER_SYS_PORT_COM	USART1

#define UART_BAUDRATE		115200

void uart_hw_init(void);
void uart_sends(UART_hw* p_hw, uint8_t* s);
char uart_receives(UART_hw* p_hw, char* s);

#endif
