/*
 * test_detect_and_sync_bp.c
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"

Cabinet_app selex_bss;
uint8_t sdo_rec_idx = 0;

int main(void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss);
	cab_app_set_state(&selex_bss, CABIN_ST_STANDBY);
	cab_app_start_id_assign(&selex_bss);
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
	switch(selex_bss.state){
	case CABIN_ST_SETUP:
	case CABIN_ST_STANDBY:
		can_receive(&can_port, can_port.rx_data);
		if(can_port.can_rx.StdId == CO_CAN_ID_TSDO + DEFAULT_BP_ID){
			cab_app_set_state(&selex_bss, CABIN_ST_ASSIGN_ID);
			selex_bss.base.assign_state = ASSIGN_ST_START;
		}
		break;
	case CABIN_ST_ASSIGN_ID:
		break;
	case CABIN_ST_ACTIVE:
		cab_list_walk_down(selex_bss.full_cab);

#if 0
		while(cab_list_walk_down(selex_bss.full_cab) != NULL){
			can_port.can_tx.StdId = RSDO_ID + selex_bss.full_cab->p_temp->data->node_id;
			HAL_CAN_AddTxMessage(&can_port.can_module, &can_port.can_tx, (uint8_t*)"c", &can_port.tx_mailbox);
		}
#endif
		break;
	case CABIN_ST_MAINTAIN:
		break;
	case CABIN_ST_FAULT:
		break;
	default:
		break;
	}

	switch(selex_bss.base.assign_state){
	case ASSIGN_ST_INACTIVE:
		break;
	case ASSIGN_ST_START:
		can_master_accept_assign_request(&selex_bss.base);
		selex_bss.base.assign_state = ASSIGN_ST_SELECT_SLAVE;
		break;
	case ASSIGN_ST_SELECT_SLAVE:
		if(can_master_select_slave(&selex_bss.base) != END_OF_LIST){
			selex_bss.base.assign_state = ASSIGN_ST_SEND_ID;
		}
		else selex_bss.base.assign_state = ASSIGN_ST_DONE;
		break;
	case ASSIGN_ST_SEND_ID:
		if(cab_list_walk_down(selex_bss.empty_cab) != NULL){
			can_master_send_id_msg(&selex_bss.base, selex_bss.empty_cab->p_temp->data->cab_id);
			selex_bss.base.assign_state = ASSIGN_ST_WAIT_CONFIRM;
		}
		else selex_bss.base.assign_state = ASSIGN_ST_DONE;
		break;
	case ASSIGN_ST_WAIT_CONFIRM:
		if(selex_bss.base.timing_state == TIMING_ST_ACTIVE){
			can_receive(&can_port, can_port.rx_data);
			if(can_port.can_rx.StdId == CO_CAN_ID_TSDO + (uint32_t)selex_bss.base.empty_slave_list->p_temp->data){
				selex_bss.base.assign_state = ASSIGN_ST_SELECT_SLAVE;
				can_master_active_node_id_pin(&selex_bss.base, selex_bss.empty_cab->p_temp->data->cab_id);
				can_master_deactive_timing_state(&selex_bss.base);
				cab_app_update_cab_node_id(&selex_bss);
				cab_app_update_cabin_list(&selex_bss);
				can_master_update_slave_list(&selex_bss.base);
			}
		}
		else{
			selex_bss.base.assign_state = ASSIGN_ST_SEND_ID;
			can_master_active_node_id_pin(&selex_bss.base, selex_bss.empty_cab->p_temp->data->cab_id);
		}
		break;
	case ASSIGN_ST_DONE:
		can_port.can_tx.StdId = CO_CAN_ID_RSDO + selex_bss.full_cab->p_head->data->node_id;
		can_port.can_tx.DLC = 1;
		*can_port.tx_data = 's';
		can_send(&can_port, can_port.tx_data);
		selex_bss.base.assign_state = ASSIGN_ST_AUTHENTIC;
		break;
	case ASSIGN_ST_AUTHENTIC:
		can_receive(&can_port, can_port.rx_data);
		if(can_port.can_rx.StdId == CO_CAN_ID_RSDO + (uint32_t)selex_bss.full_cab->p_head->data->node_id + 4){
			CO_memcpy((uint8_t*)(selex_bss.cabin[selex_bss.full_cab->p_head->data->cab_id]->bp->serial_number + sdo_rec_idx),
					can_port.rx_data, can_port.can_rx.DLC);
			if(can_port.can_rx.DLC < 8){
				*(selex_bss.cabin[selex_bss.full_cab->p_head->data->cab_id]->bp->serial_number + sdo_rec_idx) = '\0';
				cab_app_set_state(&selex_bss, CABIN_ST_ACTIVE);
			}
			else{
				sdo_rec_idx+=can_port.can_rx.DLC;
			}
		}
		break;
	default:
		break;
	}

	switch(selex_bss.base.timing_state){
	case TIMING_ST_ACTIVE:
		selex_bss.base.time_stamp--;
		if(selex_bss.base.time_stamp == 0){
			selex_bss.base.timing_state = TIMING_ST_DEACTIVE;
		}
		break;
	case TIMING_ST_DEACTIVE:
		break;
	default:
		break;
	}
}


