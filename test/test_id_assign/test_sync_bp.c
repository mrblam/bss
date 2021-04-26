/*
 * test_detect_and_sync_bp.c
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"

Cabinet_App selex_bss_app;
uint8_t sdo_rec_idx = 0;
uint8_t status;
uint8_t temp[8];

int main(void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss_app);
	cab_app_set_state(&selex_bss_app, CABIN_ST_STANDBY);
	cab_app_start_id_assign(&selex_bss_app);
	//HAL_CAN_AddTxMessage(&can_port.can_module, &can_port.can_tx, (uint8_t*)"c", &can_port.tx_mailbox);
	__enable_irq();

	while(1){

	}
}

void USART1_IRQHandler(void){
	HAL_CHECK_COM_IRQ_REQUEST(&power_sys_port.uart_module);
}

void HAL_HMI_PROCESS_DATA_IRQ(void){
	CHECK_TIM_IRQ_REQUEST(&hmi_timer);
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){
}


