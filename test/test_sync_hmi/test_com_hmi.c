/*
 * test_com_hmi.c
 *
 *  Created on: Apr 21, 2021
 *      Author: Ha
 */

/*
 * test_sync_data.c
 *
 *  Created on: Apr 15, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "cabinet_app.h"

Cabinet_app selex_bss;
BP* bp_test;
static volatile uint8_t sync_counter = 20;
static volatile uint8_t cab_id = 0;
char buff[50];
char s;
uint8_t idx, cnt, i, char_state, get_cmd_done;

int main (void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss);
	cab_app_set_state(&selex_bss, CABIN_ST_STANDBY);
	uart_receives(&power_sys_port, &s);
	__enable_irq();
	while(1){
	};
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
			char_state = idx = 0;
			get_cmd_done = 1;
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

	if(sync_counter < 20){
		sync_counter++;
	}
	else{
		switch(sync_counter){
		case 20:
			if(cab_id < CABINET_CELL_NUM){
				cab_app_sync_cab_data_hmi(&selex_bss, cab_id);
				cab_id++;
			}
			else sync_counter = 21;
			break;
		case 21:
			if(cab_list_walk_down(selex_bss.empty_cab)){
				cab_app_sync_bp_data_hmi(&selex_bss, selex_bss.full_cab->p_temp->data->bp);
			}
			else sync_counter = 22;
			break;
		case 22:
			cab_app_sync_bss_data_hmi(&selex_bss);
			sync_counter = 20;
			cab_id = 0;
			break;
		default:
			break;
		}
	}

	if(get_cmd_done == 1){
		cab_app_decode_cmd_hmi(&selex_bss, buff);
		get_cmd_done = 0;
	}
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}



