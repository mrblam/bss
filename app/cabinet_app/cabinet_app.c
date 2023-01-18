/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"
#include "uart_hw_hal.h"
#include "master_hw_hal.h"

Cabinet_App selex_bss_app;


static char tx_buff[200];
static uint32_t charge_no_cur_timestamp[2] = {0, 0};
static uint32_t swicth_time_charger = 0;
uint32_t 	sys_timestamp = 0;
//test delay sw_off after deactive bp
static uint16_t delay_time_10ms = 0;


static uint8_t cab_app_check_valid_hmi_msg(Cabinet_App* p_ca);
static void cab_app_reset_buffer(Cabinet_App* p_ca);
static void	cab_app_process_hmi_write_bss_cmd(Cabinet_App* p_ca, const uint8_t id, const uint32_t timestamp);
static void cab_app_process_hmi_write_cab_cmd(Cabinet_App* p_ca, const uint8_t id);
static void cab_app_process_hmi_write_command(Cabinet_App* p_ca, const uint8_t msg_id, const uint32_t timestamp);
static void cab_app_process_hmi_read_command(Cabinet_App* p_ca, const uint8_t msg_id);
static void cab_app_confirm_hmi_cmd(Cabinet_App* p_ca, const uint8_t msg_id, char* buff);
static uint8_t cab_app_get_obj_state(Cabinet_App* p_ca, const uint8_t msg_id);
static void cab_app_write_bss_sn(Cabinet_App* p_ca,uint8_t cab_id, const uint32_t timestamp);

void cab_app_active_charge(Cabinet_App* p_ca, uint8_t cab_id, const uint32_t timestamp){
	if(p_ca->base.CO_base.sdo_client.status != CO_SDO_RT_idle) return;
	p_ca->bss.cabs[cab_id].bp->charge_sw_state = 3;
	/*On BP*/
#if 1
	p_ca->base.bms_mainswitch_state = BMS_STATE_CHARGING;
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_MAINSWITCH, timestamp);
//	if(p_ca->base.sdo_write_object != NULL)
//	{
//		p_ca->base.sdo_write_object();
//	}
#else
	co_sdo_write_object(&p_ca->base, BMS_MAINSWITCH_INDEX,
			p_ca->bss.cabs[cab_id].bp->base.node_id,
			(uint8_t*)&p_ca->bss.cabs[cab_id].bp->charge_sw_state, 4,
			timestamp + SDO_SET_BMS_MAIN_SW_TIMEOUT_mS);
#endif
}

void cab_app_deactive_charge(Cabinet_App* p_ca, uint8_t cab_id, const uint32_t timestamp){
	if(p_ca->base.CO_base.sdo_client.status != CO_SDO_RT_idle) return;
	p_ca->bss.cabs[cab_id].bp->charge_sw_state = 0;
	/*Off BP*/
#if 1
	p_ca->base.bms_mainswitch_state = BMS_STATE_DISCHARGING;
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_MAINSWITCH, timestamp);
//	if(p_ca->base.sdo_write_object != NULL)
//	{
//		p_ca->base.sdo_write_object();
//	}
#else
	co_sdo_write_object(&p_ca->base, BMS_MAINSWITCH_INDEX,
			p_ca->bss.cabs[cab_id].bp->base.node_id,
			(uint8_t*)&p_ca->bss.cabs[cab_id].bp->charge_sw_state, 4,
			timestamp + SDO_SET_BMS_MAIN_SW_TIMEOUT_mS);
#endif
}

void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
	bp_reset_data(p_ca->bss.cabs[cab_id].bp);
	p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
	if(p_ca->bss.cabs[cab_id].op_state == CAB_CELL_ST_INIT) return;
	p_ca->bss.cabs[cab_id].op_state = CAB_CELL_ST_EMPTY;
}

void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca){   //not use
	bss_data_serialize(&p_ca->bss, tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca,uint8_t cab_id){    // not use
	if((p_ca->bss.cabs[cab_id].bp->base.con_state!=CO_SLAVE_CON_ST_CONNECTED) ||
			(p_ca->bss.cabs[cab_id].bp->vol == 0)) return;

	bp_data_serialize(p_ca->bss.cabs[cab_id].bp, tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id){ ///not use
	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], tx_buff);
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
}

void cab_app_send_msg_to_hmi(Cabinet_App* p_ca){
	(void)p_ca;
	uart_sends(&hmi_com, (uint8_t*)tx_buff);
//	uart_sends(&debug_com, (uint8_t*)tx_buff);
}

void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint32_t timestamp){
	for(uint8_t i = 0; i < p_ca->hmi_csv.valid_msg_num; i++)
	{
		switch(p_ca->hmi_csv.cmd_code[i]){
		case HMI_WRITE:
			cab_app_process_hmi_write_command(p_ca, i, timestamp);
			break;
		case HMI_READ:
			cab_app_process_hmi_read_command(p_ca, i);
			break;
		default:
			p_ca->hmi_csv.obj_state[i] = STATE_FAIL;
			break;
		}

		if(p_ca->hmi_csv.sub_obj[i] != ALL)
		{
			cab_app_confirm_hmi_cmd(p_ca, i, tx_buff);
		}
		p_ca->hmi_csv.is_new_msg_to_send[i] = 1;
		p_ca->hmi_csv.cmd_code[i] = p_ca->hmi_csv.id[i] = p_ca->hmi_csv.main_obj[i] = p_ca->hmi_csv.obj_state[i] = p_ca->hmi_csv.sub_obj[i] = 0;
		while(p_ca->hmi_csv.is_new_msg_to_send[i] != 0);
	}
	p_ca->hmi_csv.valid_msg_num = 0;
}

static void cab_app_process_hmi_write_command(Cabinet_App* p_ca, const uint8_t msg_id, const uint32_t timestamp){
	switch(p_ca->hmi_csv.main_obj[msg_id]){
	case BSS_STATION:
		cab_app_process_hmi_write_bss_cmd(p_ca, msg_id, timestamp);
		break;
	case BSS_CABINET:
		cab_app_process_hmi_write_cab_cmd(p_ca, msg_id);
		break;
	case BSS_BP:
		break;
	default:
		p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
		break;
	}
}

static void cab_app_process_hmi_read_command(Cabinet_App* p_ca, const uint8_t msg_id){
		switch(p_ca->hmi_csv.main_obj[msg_id]){
		case BSS_STATION:
			if(p_ca->hmi_csv.sub_obj[msg_id] == ALL){
				bss_data_serialize(&p_ca->bss, tx_buff);
			}
			else {
				p_ca->hmi_csv.obj_state[msg_id] = cab_app_get_obj_state(p_ca, msg_id);
				cab_app_confirm_hmi_cmd(p_ca, msg_id, tx_buff);
			}
			break;
		case BSS_CABINET:
			if(p_ca->hmi_csv.sub_obj[msg_id] == ALL){
				cab_cell_data_serialize(&p_ca->bss.cabs[p_ca->hmi_csv.id[msg_id]], tx_buff);
			}
			else{
				p_ca->hmi_csv.obj_state[msg_id] = cab_app_get_obj_state(p_ca, msg_id);
				cab_app_confirm_hmi_cmd(p_ca, msg_id, tx_buff);
			}
			break;
		case BSS_BP:
			bp_data_serialize(p_ca->bss.cabs[p_ca->hmi_csv.id[msg_id]].bp, tx_buff);
			break;
		default:
			break;
		}
}

static void	cab_app_process_hmi_write_bss_cmd(Cabinet_App* p_ca, const uint8_t msg_id, const uint32_t timestamp){
	uint8_t id 			= p_ca->hmi_csv.id[msg_id];
	uint8_t state 		= p_ca->hmi_csv.obj_state[msg_id];
	SUB_OBJS sub_obj 	= p_ca->hmi_csv.sub_obj[msg_id];

	switch(sub_obj){
	case BSS_ID_ASSIGN:
		//if(p_ca->base.assign_state == CM_ASSIGN_ST_DONE){
//			can_master_disable_pdo((CAN_master*)p_ca);
			can_master_start_assign_slave((CAN_master*)p_ca, p_ca->base.slaves[state], timestamp);
			reassign_attemp_cnt = 0;
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
//		}
//		else p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;

		break;
	case BSS_AUTHORIZE:
		if(p_ca->base.assign_state == CM_ASSIGN_ST_AUTHORIZING){
			if(state == STATE_OK){
				p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
				co_slave_set_con_state(p_ca->base.assigning_slave, CO_SLAVE_CON_ST_CONNECTED);
			}
			else if(state == STATE_FAIL){
				p_ca->base.assign_state = CM_ASSIGN_ST_FAIL;
			}
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		}
		else p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;

		break;
	case STATE:
		bss_set_state(&p_ca->bss, (BSS_STATE)state);
		if(p_ca->bss.state == BSS_ST_ACTIVE){
			if(p_ca->base.pdo_sync_timestamp == 0){
				p_ca->base.pdo_sync_timestamp = timestamp + 20;
			}
		}
		p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
		p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;

		break;
	case SYNC_DATA:
		p_ca->is_hmi_req_sync = 1;
		p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;

		break;
	case CAB_NUM:
		p_ca->bss.cab_num = state;
		p_ca->base.slave_num = p_ca->bss.cab_num;
		p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;

		break;
	case FAN:
		sw_process(&p_ca->bss.bss_fans[id], (SW_STATE)state);
		if(p_ca->bss.bss_fans[id].state == (SW_STATE)state){
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		}
		else p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;

		break;
	case LAMP:
		bss_set_led_color(&p_ca->bss, (BSS_LED_COLOR)state);

		break;
	case CHARGER:
		sw_process(&p_ca->bss.ac_chargers[id].input_power, (SW_STATE)state);
		if(p_ca->bss.ac_chargers[id].input_power.state == (SW_STATE)state){
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		}
		else p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
		break;
	default:
		p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
		break;
	}
}

static void cab_app_process_hmi_write_cab_cmd(Cabinet_App* p_ca, const uint8_t msg_id){
	SUB_OBJS sub_obj 	= p_ca->hmi_csv.sub_obj[msg_id];
	uint8_t state 		= p_ca->hmi_csv.obj_state[msg_id];
	uint8_t id 			= p_ca->hmi_csv.id[msg_id];

	switch(sub_obj){
	case DOOR:
		if(state == SW_ST_ON){
			p_ca->bss.cabs[id].door.state = DOOR_ST_OPEN;
			cab_app_delivery_bp(p_ca, id);
		}
		p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		break;

	case FAN:
		sw_process(&p_ca->bss.cabs[id].cell_fan, state);
		p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		break;

	case CHARGER:
		sw_process(&p_ca->bss.cabs[id].charger, state);
		p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		break;

	case OP_STATE:
		if(state == CAB_CELL_ST_INACTIVE){
//			cab_cell_set_op_state(&p_ca->bss.cabs[id], CAB_CELL_ST_INACTIVE);
			cab_cell_reset(&p_ca->bss.cabs[id]);
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		}
		else if(state == CAB_CELL_ST_EMPTY){
			cab_cell_set_op_state(&p_ca->bss.cabs[id], CAB_CELL_ST_EMPTY);
			co_slave_set_con_state(&p_ca->bss.cabs[id].bp->base, CO_SLAVE_CON_ST_DISCONNECT);
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		}
		else if(state == CAB_CELL_ST_INIT){
			p_ca->bss.cabs[id].op_state = CAB_CELL_ST_INIT;
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
		}
		break;

/* Use for test Slave LED */
	case LAMP:
		rs485_master_process_switch_command(p_ca->slave_com, 0, SLAVE_LED, state);
		break;

	default:
		p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
		break;
	}
}

static uint8_t cab_app_get_obj_state(Cabinet_App* p_ca, const uint8_t msg_id){
	uint8_t	obj_state;
	MAIN_OBJS bss_obj	= p_ca->hmi_csv.main_obj[msg_id];
	uint8_t obj_id 		= p_ca->hmi_csv.id[msg_id];

	switch(p_ca->hmi_csv.sub_obj[msg_id]){
	case STATE:
		obj_state = p_ca->bss.state;
		break;
	case OP_STATE:
		obj_state = p_ca->bss.cabs[obj_id].op_state;
		break;
	case DOOR:
		obj_state = p_ca->bss.cabs[obj_id].door.state;
		break;
	case CHARGER:
		if(bss_obj == BSS_STATION){
			obj_state = p_ca->bss.ac_chargers[obj_id].input_power.state;
		}
		else obj_state = p_ca->bss.cabs[obj_id].charger.state;
		break;
	case FAN:
		if(bss_obj == BSS_STATION){
			obj_state = p_ca->bss.bss_fans[obj_id].state;
		}
		else obj_state = p_ca->bss.cabs[obj_id].cell_fan.state;
		break;
	case LAMP:
		obj_state = p_ca->bss.bss_lamps[obj_id].state;
		break;
	case TEMP:
		if(bss_obj == BSS_STATION){
			obj_state = p_ca->bss.bss_temps[obj_id];
		}
		else obj_state = p_ca->bss.cabs[obj_id].temp;
		break;
	default:
		break;
	}

	return obj_state;
}

static void cab_app_confirm_hmi_cmd(Cabinet_App* p_ca, const uint8_t msg_id, char* buff){
	*buff++= ':';
	*buff++=p_ca->hmi_csv.cmd_code[msg_id];
	*buff++= ',';
	*buff++=p_ca->hmi_csv.main_obj[msg_id];
	*buff++= ',';
	buff+=long_to_string(p_ca->hmi_csv.id[msg_id], buff);
	*buff++= ',';
	*buff++=p_ca->hmi_csv.sub_obj[msg_id];
	*buff++= ',';
	if(p_ca->hmi_csv.cmd_code[msg_id] == HMI_WRITE){
		*buff++=p_ca->hmi_csv.obj_state[msg_id];
	}
	else buff+=long_to_string(p_ca->hmi_csv.obj_state[msg_id], buff);
	*buff++= '*';
	*buff++= '\n';
	*buff++= '\0';
}

static void cab_app_reset_buffer(Cabinet_App* p_ca){
	p_ca->rx_index = 0;
	for(uint8_t i = 0; i < 32; i++){
		p_ca->rx_data[i] = 0;
	}
	p_ca->is_new_msg = 0;
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
		/* Check valid msg */
		start = cab_app_search_char((char*)buff, stop + 1, p_ca->rx_index, ':');
		if(start == -1) break;
		stop = cab_app_search_char((char*)buff, stop + 2, p_ca->rx_index, '*');
		if(stop <= start) break;
		/* Parse data */
		uint8_t* token = &buff[start + 1];
		p_ca->hmi_csv.cmd_code[p_ca->hmi_csv.valid_msg_num] = *token;
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
		token += 3;
		if((*token != '*') ){
			*(token+1) = '\0';
			token -= 1;
			p_ca->hmi_csv.obj_state[p_ca->hmi_csv.valid_msg_num] = atoi((char*)token);
			token += 3;
		}
		else {
			*token = '\0';
			token -= 1;
			if((*token != STATE_OK) && (*token != STATE_FAIL)){
				p_ca->hmi_csv.obj_state[p_ca->hmi_csv.valid_msg_num] = atoi((char*)token);
			}
			else p_ca->hmi_csv.obj_state[p_ca->hmi_csv.valid_msg_num] = *token;
			token += 2;
		}
		p_ca->hmi_csv.valid_msg_num++;
	} while((start >= 0) && (stop > start));
	p_ca->is_new_msg = 0;
	cab_app_reset_buffer(p_ca);
}

void cab_app_update_charge(Cabinet_App* p_ca, const uint32_t timestamp)
{
	for(uint8_t id = 0; id < p_ca->bss.charger_num; id++)
	{
		if(p_ca->bss.ac_chargers[id].charging_cabin != NULL)
		{
			/* Process BP Fully Charged */
			if((p_ca->bss.ac_chargers[id].charging_cabin->bp->vol >= BP_STOP_CHARGER_THRESHOLD)
					|| (charge_no_cur_timestamp[id] >= 10000) )
			{
				if(p_ca->bss.ac_chargers[id].charging_cabin->bp->vol >= BP_OVER_CHARGE_THRESHOLD)
				{
//					sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);		// why sw_off before cab_deactive_charger
					cab_app_deactive_charge(p_ca, p_ca->bss.ac_chargers[id].charging_cabin->cab_id, timestamp);

					if((p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_success)){
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
					}
					if(p_ca->bss.ac_chargers[id].charging_cabin->bp->state == BP_ST_STANDBY)
//							|| (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort)) /// luu y sua lai bang cach goi ham de thay doi gia tri bien
					{
						{
							sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
//							p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
//							p_ca->base.sdo_server.state = SDO_ST_IDLE;
							p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_STANDBY;
							p_ca->bss.ac_chargers[id].charging_cabin = NULL;
							charge_no_cur_timestamp[id] = 0;
						}
					}
				}
				else //// charge_no_cur_timestamp[id] >= 10000
				{
					cab_app_deactive_charge(p_ca, p_ca->bss.ac_chargers[id].charging_cabin->cab_id, timestamp);
					if(p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_success){
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
					}
					if(p_ca->bss.ac_chargers[id].charging_cabin->bp->state == BP_ST_STANDBY)
					{
						{
							sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
//							p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
//							p_ca->base.sdo_server.state = SDO_ST_IDLE;
							p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_STANDBY;
							p_ca->bss.ac_chargers[id].charging_cabin = NULL;
							charge_no_cur_timestamp[id] = 0;
							swicth_time_charger = 0;
						}
					}
					else if(p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort)
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
				}
			}
			/* Process BP Disconnected */
			else if(p_ca->bss.ac_chargers[id].charging_cabin->bp->base.con_state == CO_SLAVE_CON_ST_DISCONNECT)
			{
				sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
				p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_EMPTY;
				p_ca->bss.ac_chargers[id].charging_cabin = NULL;
				charge_no_cur_timestamp[id] = 0;
			}
			/* Process Active charging cabin */
			else{
				if(cab_cell_get_op_state(p_ca->bss.ac_chargers[id].charging_cabin) == CAB_CELL_ST_CHARGING){
					if(p_ca->bss.ac_chargers[id].charging_cabin->bp->cur < 100) charge_no_cur_timestamp[id] += 10;
					else{
//						swicth_time_charger +=10;
						charge_no_cur_timestamp[id] = 0;
					}
					continue;
				}
				cab_app_active_charge(p_ca, p_ca->bss.ac_chargers[id].charging_cabin->cab_id, timestamp);
				if(p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_success || p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort){
					p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
				}
				if(p_ca->bss.ac_chargers[id].charging_cabin->bp->base.node_id !=
						p_ca->base.sdo_server.node_id_processing) continue;
				if(p_ca->bss.ac_chargers[id].charging_cabin->bp->state == BP_ST_CHARGING){
					p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_CHARGING;
				}
			}
		}
		else
		{
	 		Cabinet* cab = bss_get_cab_need_charge(&p_ca->bss, id);
			if(cab != NULL){
				delay_time_10ms ++;
				if(delay_time_10ms > 500){					// delay 5s
					delay_time_10ms = 0;
					p_ca->bss.ac_chargers[id].charging_cabin = cab;
					sw_on(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
				}
//				if(p_ca->bss.ac_chargers[id].charging_cabin->charger.state == SW_ST_ON){
//					p_ca->bss.ac_chargers[id].charging_cabin = cab;
//				}
			}

		}
	}
}

void cab_app_update_connected_cab_state(Cabinet_App* p_app){
	for(uint8_t id = 0; id < p_app->bss.cab_num; id++){
		if((p_app->bss.cabs[id].bp->base.con_state == CO_SLAVE_CON_ST_CONNECTED)
				&& (p_app->base.pdo_sync_timestamp)){
			p_app->bss.cabs[id].bp->base.inactive_time_ms += APP_STATE_MACHINE_UPDATE_TICK_mS;
			if(p_app->bss.cabs[id].bp->base.inactive_time_ms <= BP_INACTIVE_TIMEOUT_mS) continue;
			cab_cell_reset(&p_app->bss.cabs[id]);
//			p_app->bss.cabs[id].op_state = CAB_CELL_ST_FAIL;
			for(uint8_t i = 0; i < p_app->bss.charger_num; i++){
				if(p_app->bss.ac_chargers[i].charging_cabin != &p_app->bss.cabs[id]) continue;
				sw_off(&p_app->bss.ac_chargers[i].charging_cabin->charger);
				p_app->bss.ac_chargers[id].charging_cabin = NULL;
			}
		}
	}
}

static void cab_app_write_bss_sn(Cabinet_App* p_ca,uint8_t cab_id, const uint32_t timestamp){
//	if(p_ca->base.CO_base.sdo_client.status != CO_SDO_RT_idle) return;
	/*Off BP*/
	p_ca->base.bss_sn[0] = 'b';
	p_ca->base.bss_sn[1] = 's';
	p_ca->base.bss_sn[2] = 's';
	p_ca->base.bss_sn[3] = '_';
	p_ca->base.bss_sn[4] = 'v';
	p_ca->base.bss_sn[5] = '1';
	p_ca->base.bss_sn[6] = '.';
	p_ca->base.bss_sn[7] = '0';
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_MATED_DEV, timestamp);
}
