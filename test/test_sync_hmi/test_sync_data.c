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

int main (void){
	board_init();
	cab_app_init(&selex_bss);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	if(htim->Instance == TIM2){
		if(cnt < 15){
			cab_cell_data_serialize(selex_bss->cabin[cnt], buff);
			cnt++;
		}
		else if(cnt < 30){
			bp_data_serialize(selex_bss->cabin[cnt-15]->bp, buff);
			cnt++;
		}
		else {
			bss_data_serialize(selex_bss->bss_data, buff);
			cnt = 0;
		}

		while(*(buff+i)!= '\0'){
			HAL_UART_Transmit(&huart1, (uint8_t*)(data + i), 1, 1000);
			i++;
		}
			i = 0;
	}
}
