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

void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca){
	bss_data_serialize(&p_ca->bss, tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca,uint8_t cab_id){
	if(p_ca->bss.cabs[cab_id].bp->base.con_state!=CO_SLAVE_CON_ST_CONNECTED) return;

	bp_data_serialize(p_ca->bss.cabs[cab_id].bp, tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id){
	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
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

static void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint8_t cab_id,
		const char obj, const uint8_t state){
	switch(obj){
	case 'D':
		if(state == 1)	sw_on(&p_ca->bss.cabs[cab_id].door.solenoid);
		break;
	case 'F':
		if(state == 1)	sw_on(&p_ca->bss.cabs[cab_id].cell_fan);
		else if(state == 0)	sw_off(&p_ca->bss.cabs[cab_id].cell_fan);
		break;
	case 'S':
		p_ca->bss.cabs[cab_id].state = state;
		break;
	case 'X':
		p_ca->base.assign_state = state;
		break;
	default:
		break;
	}
}

static void cab_app_reset_buffer(Cabinet_App* p_ca){
	p_ca->rx_index = 0;
	for(uint8_t i = 0; i < 32; i++){
		p_ca->rx_data[i] = 0;
	}
}
