/*
 * test_process_cmd.c
 *
 *  Created on: Apr 20, 2021
 *      Author: Ha
 */

#include "cabinet_app.h"

Cabinet_app selex_bss;
uint8_t char_state = 0;
uint8_t get_cmd_done = 0;
char buff[32]={0};
char s;
uint8_t idx = 0;

int main(void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss);

	uart_receives(&power_sys_port, &s);
	__enable_irq();
	while(1){

	}
}

void USART1_IRQHandler(void){
	HAL_CHECK_COM_IRQ_REQUEST(&power_sys_port.uart_module);

	switch(char_state){
	case 0:
		if(s == ':'){
			char_state = 1;
		}
		break;
	case 1:
		if(s == 'W'){
			char_state = 2;
		}
		break;
	case 2:
		if(s == '*'){
			char_state = 0;
			get_cmd_done = 1;
			idx = 0;
			break;
		}
		*(buff + idx) = s;
		idx++;
		break;
	default:
		break;
	}

	uart_receives(&power_sys_port, &s);
}

void HAL_HMI_PROCESS_DATA_IRQ(void){
	CHECK_TIM_IRQ_REQUEST(&hmi_timer);

	if(get_cmd_done == 1){

			cab_app_decode_cmd_hmi(&selex_bss, buff);
			get_cmd_done = 0;

	}
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}


