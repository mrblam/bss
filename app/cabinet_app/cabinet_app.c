/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"
#include "uart_hw_hal.h"

static char tx_buff[50];

static uint8_t cab_app_check_valid_hmi_msg(Cabinet_App* p_ca);
static void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint8_t cab_id,
		const char obj, const uint8_t state);
static void cab_app_reset_buffer(Cabinet_App* p_ca);

void cab_app_active_charge(Cabinet_App* p_ca,uint8_t cab_id){
	p_ca->bss.cabs[cab_id].bp->charge_sw_state=3;
	co_sdo_write_object(&p_ca->base, BMS_MAINSWITCH_INDEX,
			p_ca->bss.cabs[cab_id].bp->base.node_id,
			(uint8_t*)&p_ca->bss.cabs[cab_id].bp->charge_sw_state,4,0);
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
	bss_data_serialize(&p_ca->bss, tx_buff);
	uart_sends(&power_sys_port, (uint8_t*)tx_buff);
}

void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca,uint8_t cab_id){
	if(p_ca->bss.cabs[cab_id].bp->base.con_state!=CO_SLAVE_CON_ST_CONNECTED) return;

	bp_data_serialize(p_ca->bss.cabs[cab_id].bp, tx_buff);
	uart_sends(&power_sys_port, (uint8_t*)tx_buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id){

	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], tx_buff);
	uart_sends(&power_sys_port, (uint8_t*)tx_buff);
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
	uint8_t id = string_to_long(token);
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

static uint8_t cab_app_check_valid_hmi_msg(Cabinet_App* p_ca){
	uint8_t* buff = (uint8_t*)p_ca->rx_data;
	buff += p_ca->rx_index-1;
	while((*buff != '*') || (*buff != '\0')){
		if(*buff == '*'){
			*buff = '\0';
			buff--;
			while((*buff != ':') || (*buff != '\0')){
				if(*buff == ':'){
					p_ca->start_msg_index = ++buff;
					return 1;
				}
				else if(*buff == '\0') return 0;
				buff--;
			}
		}
		else if(*buff == '\0') return 0;
		buff--;
	}
	return 0;
}

void cab_app_parse_hmi_msg(Cabinet_App* p_ca){
	if(cab_app_check_valid_hmi_msg(p_ca)){
		if(*p_ca->start_msg_index == 'W'){
			char* token = strtok((char*)++p_ca->start_msg_index,",");
			token = strtok(NULL, ",");
			uint8_t cab_id = string_to_long(token);
			token = strtok(NULL, ",");
			char obj = *token;
			token = strtok(NULL, ",");
			uint8_t state = string_to_long(token);
			cab_app_process_hmi_command(p_ca, cab_id, obj, state);
		}
		cab_app_reset_buffer(p_ca);
	}
}

static void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint8_t cab_id, const char obj, const uint8_t state){
	if(state == 1){
		switch(obj){
		case 'D':
			if(state == 1){
				sw_on(&p_ca->bss.cabs[cab_id].door.solenoid);
				p_ca->is_new_msg = 0;
			}
			break;
		case 'F':
			break;
		case 'C':
			break;
		case 'S':
			break;
		}
	}
}

static void cab_app_reset_buffer(Cabinet_App* p_ca){
	p_ca->rx_index = 0;
	for(uint8_t i = 0; i < 32; i++){
		p_ca->rx_data[i] = 0;
	}
}
