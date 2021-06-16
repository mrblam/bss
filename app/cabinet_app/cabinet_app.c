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
static void	cab_app_process_hmi_bss_cmd(Cabinet_App* p_ca, const uint8_t id, const uint32_t timestamp);
static void cab_app_process_hmi_cab_cmd(Cabinet_App* p_ca, const uint8_t id);
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
	Cabinet* cab = &p_ca->bss.ac_chargers[charger_id].assigned_cabs[0];
	uint8_t avai_cab_num = 0;		/* Available Cabinet Number */

	/* Get BP which has highest voltage to charge first */
	for(uint8_t i = 0; i < p_ca->bss.ac_chargers[charger_id].assigned_cab_num; i++){
		if((p_ca->bss.ac_chargers[charger_id].assigned_cabs[i].bp->vol > 0) &&
				(p_ca->bss.ac_chargers[charger_id].assigned_cabs[i].bp->vol < BP_START_CHARGE_THRESHOLD) &&
				(p_ca->bss.ac_chargers[charger_id].assigned_cabs[i].op_state != CAB_CELL_ST_CHARGING)){
			avai_cab_num++;
			if(cab->bp->vol < p_ca->bss.ac_chargers[charger_id].assigned_cabs[i].bp->vol){
				cab = &p_ca->bss.ac_chargers[charger_id].assigned_cabs[i];
			}
		}
	}
	if(avai_cab_num == 0) return NULL;
	else return cab;
}

#if 0
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
#endif

void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint32_t timestamp){
	for(uint8_t i = 0; i < p_ca->hmi_csv.valid_msg_num; i++){
		switch(p_ca->hmi_csv.main_obj[i]){
		case BSS_STATION:
			cab_app_process_hmi_bss_cmd(p_ca, i, timestamp);
			break;
		case BSS_CABINET:
			cab_app_process_hmi_cab_cmd(p_ca, i);
			break;
		case BSS_BP:
		default:
			break;
		}
	}
	p_ca->hmi_csv.valid_msg_num = 0;
}

static void	cab_app_process_hmi_bss_cmd(Cabinet_App* p_ca, const uint8_t id, const uint32_t timestamp){
	uint8_t state;
	switch(p_ca->hmi_csv.sub_obj[id]){
	case BSS_ID_ASSIGN:
		if(p_ca->base.assign_state == CM_ASSIGN_ST_DONE){
			can_master_start_assign_slave((CAN_master*)p_ca, p_ca->base.slaves[p_ca->hmi_csv.id[id]], timestamp);
		}
		break;
	case BSS_AUTHORIZE:
		if(p_ca->base.assign_state == CM_ASSIGN_ST_AUTHORIZING){
			if(p_ca->hmi_csv.obj_state[id] == AUTH_OK){
				p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
				co_slave_set_con_state(p_ca->base.assigning_slave, CO_SLAVE_CON_ST_CONNECTED);
				p_ca->base.pdo_sync_timestamp = timestamp + 10;
			}
			else if(p_ca->hmi_csv.obj_state[id] == AUTH_FAIL){
				p_ca->base.assign_state = CM_ASSIGN_ST_FAIL;
			}
			p_ca->base.assigning_slave = NULL;
		}
		break;
	case STATE:
		//state = string_to_long((char*)&p_ca->hmi_csv.obj_state);
		state = atoi((char*)&p_ca->hmi_csv.obj_state[id]);
		bss_set_state(&p_ca->bss, (BSS_STATE)state);
		break;
	case SYNC_DATA:
		p_ca->is_hmi_req_sync = 1;
		break;
	case CAB_NUM:
		state = atoi((char*)&p_ca->hmi_csv.obj_state[id]);
		p_ca->bss.cab_num = state;
		p_ca->base.slave_num = p_ca->bss.cab_num;
		break;
	case FAN:
		sw_process(&p_ca->bss.bss_fans[p_ca->hmi_csv.id[id]], (SW_STATE)atoi((char*)&p_ca->hmi_csv.obj_state[id]));
		break;
	case LAMP:
		sw_process(&p_ca->bss.bss_lamps[p_ca->hmi_csv.id[id]], (SW_STATE)atoi((char*)&p_ca->hmi_csv.obj_state[id]));
		break;
	case CHARGER:
		sw_process(&p_ca->bss.ac_chargers[p_ca->hmi_csv.id[id]].input_power, (SW_STATE)atoi((char*)&p_ca->hmi_csv.obj_state[id]));
		break;
	default:
		break;
	}
}

static void cab_app_process_hmi_cab_cmd(Cabinet_App* p_ca, const uint8_t id){
	uint8_t state = atoi((char*)&p_ca->hmi_csv.obj_state[id]);
	switch(p_ca->hmi_csv.sub_obj[id]){
	case DOOR:
		if(p_ca->hmi_csv.obj_state[id] == 1){
			cab_app_delivery_bp(p_ca, p_ca->hmi_csv.id[id]);
		}
		break;
	case FAN:
		sw_process(&p_ca->bss.cabs[p_ca->hmi_csv.id[id]].cell_fan, state);
		break;
	case CHARGER:
		sw_process(&p_ca->bss.cabs[p_ca->hmi_csv.id[id]].charger, state);
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

static int8_t cab_app_search_char(char* buff, uint8_t start, uint8_t stop, char find_char){
	for(uint8_t i = start; i < stop; i++){
		if(buff[i] == find_char) return i;
	}
	return -1;
}

void cab_app_check_buffer(Cabinet_App* p_ca){
	uint8_t* buff = (uint8_t*)p_ca->rx_data;
	int8_t start = -1;
	int8_t stop  = -1;

	do{
		start = cab_app_search_char((char*)buff, stop + 1, p_ca->rx_index, ':');
		if(start == -1) break;
		stop = cab_app_search_char((char*)buff, stop + 2, p_ca->rx_index, '*');
		if(stop <= start) break;

		uint8_t* token = &buff[start + 1];
		if((*token == 'W') || (*token == 'R')){
			token += 2;
			p_ca->hmi_csv.main_obj[p_ca->hmi_csv.valid_msg_num] = *token;
			token += 3;
			if(*token != ','){
				*(token+1) = '\0';
				token -= 1;
				p_ca->hmi_csv.id[p_ca->hmi_csv.valid_msg_num] = atoi((char*)token);
				token += 3;
			}
			else {
				*token = '\0';
				token -= 1;
				p_ca->hmi_csv.id[p_ca->hmi_csv.valid_msg_num] = atoi((char*)token);
				token += 2;
			}
			p_ca->hmi_csv.sub_obj[p_ca->hmi_csv.valid_msg_num] = *token;
			token += 2;
			p_ca->hmi_csv.obj_state[p_ca->hmi_csv.valid_msg_num] = *token;
		}
		p_ca->hmi_csv.valid_msg_num++;
	} while((start >= 0) && (stop > start));

	p_ca->is_new_msg = 0;
	cab_app_reset_buffer(p_ca);
}
#if 0
void cab_app_parse_hmi_msg(Cabinet_App* p_ca){
	if(cab_app_check_valid_hmi_msg(p_ca)){
		char* token = strtok((char*)p_ca->start_msg_index, ",");
		if((*token == 'W') || (*token == 'R')){
			token = strtok(NULL, ",");
			p_ca->hmi_csv.main_obj = *token;
			token = strtok(NULL, ",");
			p_ca->hmi_csv.id = string_to_long(token);
			token = strtok(NULL, ",");
			p_ca->hmi_csv.sub_obj = *token;
			token = strtok(NULL, ",");
			p_ca->hmi_csv.obj_state = *token;
		}
		p_ca->hmi_csv.is_new_data = 1;
	}
	cab_app_reset_buffer(p_ca);
}
#endif
