#include "debug_com_port_hardware.h"

#include "stdio.h"
#include "stdlib.h"

DEBUG_COM_HW debug_port;

void debug_com_hw_init(void){
}

void uart_sends(DEBUG_COM_HW* p_hw,const uint8_t* s){

    while(*s){
    	      s++;
    }
}
//Uart_Receive_Handle USART2_Receive_Handle;

///////////
void Delay(__IO uint32_t nTime) {
	uint32_t a = nTime * 480;
	while (a-- != 0);
}

///////////

#if 0
void DEBUG_PORT_COM_HANLDE(void){
	if(USART_GetITStatus(DEBUG_PORT_COM, USART_IT_RXNE) != RESET)
	{
		if(USART2_Receive_Handle != NULL){
			USART2_Receive_Handle();
		}

	}
}

void debug_port_set_receive_handle(Uart_Receive_Handle handle){
	USART2_Receive_Handle =handle;
}
#endif

