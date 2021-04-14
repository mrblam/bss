#ifndef DEBUG_PORT_H_
#define DEBUG_PORT_H_
#include "stdint.h"

typedef struct UART_HW_t UART_HW;
struct UART_HW_t{
};

extern UART_HW power_sync_port;
extern UART_HW	hmi_sync_port;

void uart_hw_init(void);
void uart_sends(UART_HW* p_hw,const uint8_t* s);

void debug_com_hw_init(void);
//void debug_port_set_receive_handle(Uart_Receive_Handle handle);
//void USART_Write_String(char *a);
void uart_sends(UART_HW* p_hw,const uint8_t* s);

#endif
