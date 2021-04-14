#include "debug_com_port_hardware.h"

#include "stdio.h"
#include "stdlib.h"

UART_HW power_sync_port;
UART_HW	hmi_sync_port;



void uart_hw_init(void){
	power_sync_hw_gpio_init();
	hmi_sync_hw_gpio_init();
}

void power_sync_hw_gpio_init(void){
}

void hmi_sync_hw_gpio_init(void){
}

void uart_sends(UART_HW* p_hw,const uint8_t* s){

        (void)p_hw;
    while(*s){
    	      s++;
    }
}
