/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"
#include "uart_hw_hal.h"

static char tx_buff[200];

static uint8_t cab_app_check_valid_hmi_msg(Cabinet_App* p_ca);
static void cab_app_reset_buffer(Cabinet_App* p_ca);
static void	cab_app_process_hmi_bss_cmd(Cabinet_App* p_ca, const uint32_t timestamp);
static void cab_app_process_hmi_cab_cmd(Cabinet_App* p_ca);
static Cabinet* cab_app_get_cab_need_charge(Cabinet_App* p_ca, uint8_t charger_id);

void cab_app_active_charge(Cabinet_App* p_ca,uint8_t cab_id){
	p_ca->bss.cabs[cab_id].bp->charge_sw_state = 3;
	co_sdo_write_object(&p_ca->base, BMS_MAINSWITCH_INDEX,
			p_ca->bss.cabs[cab_id].bp->base.node_id,
			(uint8_t*)&p_ca->bss.cabs[cab_id].bp->charge_sw_state,4,0);
}

void cab_app_deactive_charge(Cabinet_App* p_ca, CABIN_ID cab_id){
	//cab_cell_deactive_charger(&p_ca->bss.cabs[cab_id]);
}

void cab_app_receive_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
	//cab_cell_update_door_state(&p_ca->bss.cabs[cab_id]);
}

void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
}

void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca){
	bss_data_serialize(&p_ca->bss, tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca,uint8_t cab_id){
	if((p_ca->bss.cabs[cab_id].bp->base.con_state!=CO_SLAVE_CON_ST_CONNECTED) ||
			(p_ca->bss.cabs[cab_id].bp->vol == 0)) return;

	bp_data_serialize(p_ca->bss.cabs[cab_id].bp, tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id){
	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

static Cabinet* cab_app_get_cab_need_charge(Cabinet_App* p_ca, uint8_t charger_id){
	for(uint8_t i = 0; i < p_ca->bss.ac_chargers[charger_id].assigned_cab_num; i++){
		if((p_ca->bss.ac_chargers[charger_id].assigned_cabs[i].bp->vol > 0) &&
				(p_ca->bss.ac_chargers[charger_id].assigned_cabs[i].bp->vol < BP_START_CHARGE_THRESHOLD)){
			return &p_ca->bss.ac_chargers[charger_id].assigned_cabs[i];
		}
	}
	return NULL;
}

#if 0
void cab_app_update_charging_state(Cabinet_App* p_ca, const uint32_t timestamp){
	for(uint8_t i = 0; i < CHARGER_NUM; i++){
		if(p_ca->bss.ac_chargers[i].charging_cabin == NULL){

		}
	}
}
#endif

void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint32_t timestamp){
	if(p_ca->hmi_csv.is_new_data == 1){
		switch(p_ca->hmi_csv.main_obj){
		case BSS_STATION:
			cab_app_process_hmi_bss_cmd(p_ca, timestamp);
			break;
		case BSS_CABINET:
			cab_app_process_hmi_cab_cmd(p_ca);
			break;
		case BSS_BP:
		default:
			break;
		}
		p_ca->hmi_csv.is_new_data = 0;
	}
}

static void	cab_app_process_hmi_bss_cmd(Cabinet_App* p_ca, const uint32_t timestamp){
	switch(p_ca->hmi_csv.sub_obj){
	case BSS_ID_ASSIGN:
		can_master_start_assign_slave((CAN_master*)p_ca, p_ca->base.slaves[p_ca->hmi_csv.id], timestamp);
		break;
	case BSS_AUTHORIZE:
		if(p_ca->hmi_csv.obj_state == AUTH_OK){
			p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
			co_slave_set_con_state(p_ca->base.assigning_slave, CO_SLAVE_CON_ST_CONNECTED);
			p_ca->base.pdo_sync_timestamp = timestamp + 10;
		}
		else if(p_ca->hmi_csv.obj_state == AUTH_FAIL){
			p_ca->base.assign_state = CM_ASSIGN_ST_FAIL;
		}
		p_ca->base.assigning_slave = NULL;
		break;
	case FAN:
		sw_process(&p_ca->bss.bss_fans[p_ca->hmi_csv.id],
				(SW_STATE)string_to_long((char*)&p_ca->hmi_csv.obj_state));
	case LAMP:
		sw_process(&p_ca->bss.bss_lamps[p_ca->hmi_csv.id],
				(SW_STATE)string_to_long((char*)&p_ca->hmi_csv.obj_state));
		break;
	case CHARGER:
		sw_process(&p_ca->bss.ac_chargers[p_ca->hmi_csv.id].input_power,
				(SW_STATE)string_to_long((char*)&p_ca->hmi_csv.obj_state));
	default:
		break;
	}
}

static void cab_app_process_hmi_cab_cmd(Cabinet_App* p_ca){
	SW_STATE state = (SW_STATE)string_to_long((char*)&p_ca->hmi_csv.obj_state);
	switch(p_ca->hmi_csv.sub_obj){
	case DOOR:
		if(p_ca->hmi_csv.obj_state == SW_ACTIVE){
			cab_app_delivery_bp(p_ca, p_ca->hmi_csv.id);
		}
		break;
	case FAN:
		sw_process(&p_ca->bss.cabs[p_ca->hmi_csv.id].cell_fan, state);
		break;
	case CHARGER:
		sw_process(&p_ca->bss.cabs[p_ca->hmi_csv.id].charger, state);
	default:
		break;
	}
}

static void cab_app_reset_buffer(Cabinet_App* p_ca){
	p_ca->rx_index = 0;
	for(uint8_t i = 0; i < 32; i++){
		p_ca->rx_data[i] = 0;
	}
	p_ca->is_new_msg = 0;
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
		char* token = strtok((char*)p_ca->start_msg_index, ",");
		if(*token == 'W'){
			token = strtok(NULL, ",");
			p_ca->hmi_csv.main_obj = *token;
			token = strtok(NULL, ",");
			p_ca->hmi_csv.id = string_to_long(token);
			token = strtok(NULL, ",");
			p_ca->hmi_csv.sub_obj = *token;
			token = strtok(NULL, ",");
			p_ca->hmi_csv.obj_state = *token;
		}
		p_ca->is_new_msg = 0;
		p_ca->hmi_csv.is_new_data = 1;
		cab_app_reset_buffer(p_ca);
	}
}
