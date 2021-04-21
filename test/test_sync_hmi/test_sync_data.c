/*
 * test_sync_data.c
 *
 *  Created on: Apr 15, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "cabinet_app.h"

Cabinet_app selex_bss;
static volatile uint8_t sync_counter = 20;
static volatile uint8_t cab_id = 0;
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
			if(cab_list_walk_down(selex_bss.full_cab)){
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
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}
