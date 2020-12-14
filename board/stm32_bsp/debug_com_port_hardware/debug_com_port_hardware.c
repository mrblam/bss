#include "debug_com_port_hardware.h"

#include "stdio.h"
#include "stdlib.h"

DEBUG_COM_HW debug_port;

void debug_com_hw_init(void){
}

void uart_sends(DEBUG_COM_HW* p_hw,const uint8_t* s){

        (void)p_hw;
    while(*s){
    	      s++;
    }
}
