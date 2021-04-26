/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"
#include "uart_hw_hal.h"

void cab_app_active_charge(Cabinet_App* p_ca,CABIN_ID cab_id){
	cab_cell_active_charger(&p_ca->bss.cabs[cab_id]);
}

void cab_app_deactive_charge(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_deactive_charger(&p_ca->bss.cabs[cab_id]);
}

void cab_app_receive_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
	cab_cell_update_door_state(&p_ca->bss.cabs[cab_id]);
}

void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
}

void cab_app_check_bp_state(Cabinet_App* p_ca, CABIN_ID cab_id){

}

void cab_app_update_tilt_ss(Cabinet_App* p_ca){

}

void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca){
	char buff[50];

	bss_data_serialize(&p_ca->bss, buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_sync_bp_data_hmi(__attribute__((unused)) Cabinet_App* p_ca, BP* p_bp){
	char buff[50];

	bp_data_serialize(p_bp, buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id){
	char buff[50];

	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_decode_cmd_hmi(Cabinet_App* p_ca, char* buff){
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

void cab_app_process_bss_cmd_hmi(__attribute__((unused)) Cabinet_App* p_ca, char* token){
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

void cab_app_process_cab_cmd_hmi(__attribute__((unused)) Cabinet_App* p_ca, char* token){
	token = strtok(NULL, ",");
	uint8_t id = string_to_long(token) + 1;
	token = strtok(NULL, ",");
	char* obj = token;
	token = strtok(NULL, ",");
	uint8_t state = string_to_long(token);

	switch(*obj){
	case 'F':
		if(state == 1){
			cab_cell_fan_turn_on(&p_ca->bss.cabs[id]);
		}
		else {
			cab_cell_fan_turn_off(&p_ca->bss.cabs[id]);
		}
		break;
	case 'D':
		if(state == 1){
			cab_cell_open_door(&p_ca->bss.cabs[id]);
		}
		break;
	case 'C':
		if(state == 1){
			cab_cell_active_charger(&p_ca->bss.cabs[id]);
		}
		else {
			cab_cell_deactive_charger(&p_ca->bss.cabs[id]);
		}
		break;
	default:
		break;
	}
}
