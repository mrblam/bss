/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"

void node_id_pin_active(uint8_t cab_id);
void node_id_pin_deactive(uint8_t cab_id);

void cab_app_init(Cabinet_app* p_ca){
	can_master_init((CAN_master*) p_ca);
	p_ca->state = CABIN_ST_SETUP;
	p_ca->timing_state = TIMING_ST_DEACTIVE;
	p_ca->empty_cab = cab_list_init();
	p_ca->full_cab = cab_list_init();

	for(uint8_t cab_id = CAB1 - 1; cab_id < CAB7; cab_id++){
		p_ca->cabin[cab_id] = cab_cell_construct();
		p_ca->cabin[cab_id]->cab_id = cab_id + 1;
		p_ca->cabin[cab_id]->node_id = DEFAULT_BP_ID;
		cab_list_insert_to_tail(p_ca->empty_cab, p_ca->cabin[cab_id]);
	}

	p_ca->ioe_cfan = ioe_construct();
	p_ca->ioe_sol = ioe_construct();
	p_ca->bss_data = bss_data_construct();

	p_ca->base.active_node_id_pin = node_id_pin_active;
	p_ca->base.deactive_node_id_pin = node_id_pin_deactive;
}

void cab_app_set_state(Cabinet_app* p_ca, CABIN_STATE state){
	p_ca->state = state;
}

CABIN_STATE cab_app_get_state(Cabinet_app* p_ca){
	return 0;
}

/* Enable timeout process */
void cab_app_active_timing_state(Cabinet_app* p_ca, uint16_t time_ms){
	p_ca->timing_state = TIMING_ST_ACTIVE;
	p_ca->time_stamp = time_ms;
}

/* Disable timeout process */
void cab_app_deactive_timing_state(Cabinet_app* p_ca){
	p_ca->timing_state = TIMING_ST_DEACTIVE;
	p_ca->time_stamp = 0;
}

void cab_app_active_charge(Cabinet_app* p_ca,CABIN_ID cab_id){

}

void cab_app_deactive_charge(Cabinet_app* p_ca, CABIN_ID cab_id){

}

void cab_app_receive_bp(Cabinet_app* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(p_ca->cabin[cab_id]);
	cab_cell_check_door_state(p_ca->cabin[cab_id]);
}

void cab_app_delivery_bp(Cabinet_app* p_ca, CABIN_ID cab_id){

}

void cab_app_check_bp_state(Cabinet_app* p_ca, CABIN_ID cab_id){

}

void cab_app_update_tilt_ss(Cabinet_app* p_ca){

}

void cab_app_update_cab_node_id(Cabinet_app* p_ca){
	p_ca->empty_cab->p_temp->data->node_id = p_ca->base.empty_slave_list->p_temp->data;
}

void cab_app_update_cabin_list(Cabinet_app* p_ca){
	cab_list_remove_node(p_ca->empty_cab, p_ca->empty_cab->p_temp->data);
	cab_list_insert_to_tail(p_ca->full_cab, p_ca->empty_cab->p_temp->data);
}

void cab_app_sync_bss_data_hmi(Cabinet_app* p_ca){
	char buff[50];

	bss_data_serialize(p_ca->bss_data, buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_sync_bp_data_hmi(__attribute__((unused)) Cabinet_app* p_ca, BP* p_bp){
	char buff[50];

	bp_data_serialize(p_bp, buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_app* p_ca, uint8_t cab_id){
	char buff[50];

	cab_cell_data_serialize(p_ca->cabin[cab_id], buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_decode_cmd_hmi(Cabinet_app* p_ca, char* buff){
	char* token;

	token = strtok(buff, ",");
	switch(*token){
	case 'C':
		cab_app_process_cab_cmd_hmi(p_ca, token);
		break;
	case 'B':
		break;
	case 'S':
		break;
	default:
		break;
	}
}

void cab_app_process_bss_cmd_hmi(__attribute__((unused)) Cabinet_app* p_ca, char* token){
	token = strtok(NULL, ",");
	char* obj = token;
	token = strtok(NULL, ",");
	uint8_t state = string_to_long(token);

	switch(*obj){
	case 'F':
		break;
	case 'C':
		break;
	case 'L':
		break;
	default:
		break;
	}
}

void cab_app_process_cab_cmd_hmi(__attribute__((unused)) Cabinet_app* p_ca, char* token){
	token = strtok(NULL, ",");
	uint8_t id = string_to_long(token);
	token = strtok(NULL, ",");
	char* obj = token;
	token = strtok(NULL, ",");
	uint8_t state = string_to_long(token);

	switch(*obj){
	case 'F':
		if(state == 1){
			cab_cell_active_cell_fan(p_ca->cabin[id]);
		}
		else {
			cab_cell_deactive_cell_fan(p_ca->cabin[id]);
		}
		break;
	case 'D':
		if(state == 1){
			cab_cell_open_door(p_ca->cabin[id]);
		}
		break;
	case 'C':
		if(state == 1){
			cab_cell_active_charger(p_ca->cabin[id]);
		}
		else {
			cab_cell_deactive_charger(p_ca->cabin[id]);
		}
		break;
	default:
		break;
	}
}

void cab_app_start_id_assign(Cabinet_app* p_ca){
	Cabinet_node* p_temp = cab_list_walk_down(p_ca->empty_cab);
	while(p_temp != NULL){
		can_master_active_node_id_pin((CAN_master*)p_ca, p_temp->data->cab_id);
		p_temp = cab_list_walk_down(p_ca->empty_cab);
	}
	free(p_temp);
}

/* ----------------------------------------------------------------------------------------------*/

void node_id_pin_active(uint8_t cab_id){
	switch(cab_id){
	case CAB1:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		break;
	case CAB2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		break;
	case CAB3:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		break;
	case CAB4:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		break;
	case CAB5:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		break;
	case CAB6:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case CAB7:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		break;
	case CAB8:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		break;
	case CAB9:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		break;
	case CAB10:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		break;
	case CAB11:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case CAB12:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
		break;
	case CAB13:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		break;
	case CAB14:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		break;
	case CAB15:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void node_id_pin_deactive(uint8_t cab_id){
	switch(cab_id){
	case CAB1:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		break;
	case CAB2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		break;
	case CAB3:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		break;
	case CAB4:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break;
	case CAB5:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case CAB6:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case CAB7:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		break;
	case CAB8:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		break;
	case CAB9:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		break;
	case CAB10:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		break;
	case CAB11:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
		break;
	case CAB12:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
		break;
	case CAB13:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
		break;
	case CAB14:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		break;
	case CAB15:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}
