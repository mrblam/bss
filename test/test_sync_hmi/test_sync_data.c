/*
 * test_sync_data.c
 *
 *  Created on: Apr 15, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "cabinet_app.h"
#include "interrupt_hal.h"

Cabinet_app selex_bss;
char buff[50];
uint8_t cnt = 0;
uint8_t i = 0;

int main (void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss);
	__enable_irq();
	while(1){

	};
}

void SysTick_Handler(void){
#if 1
		if(cnt < 15){
			cab_cell_data_serialize(selex_bss.cabin[cnt], buff);
			cnt++;
		}
		else if(cnt < 30){
			bp_data_serialize(selex_bss.cabin[cnt-15]->bp, buff);
			cnt++;
		}
		else {
			bss_data_serialize(selex_bss.bss_data, buff);
			cnt = 0;
		}
		while(*(buff+i)!= '\0'){
			HAL_UART_Transmit(&uart_power, (uint8_t*)(buff + i), 1, 1000);
			i++;
		}
			i = 0;
#endif
}
