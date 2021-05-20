/*
 * test_can.c
 *
 *  Created on: May 19, 2021
 *      Author: KhanhDinh
 */

#include "board.h"

uint8_t data = 'A';

int main(void){
	__disable_irq();
	HAL_Init();
	core_hw_init();
	can_hardware_init();
	__enable_irq();
	while(1){
		can_send(&can_port, &data);
		for(uint32_t cnt = 0; cnt < 1000000; cnt++);
	}
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}
