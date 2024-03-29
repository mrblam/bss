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
static char data_log[200];
static uint32_t charge_no_cur_timestamp[2] = { 0, 0 };
static uint32_t swicth_time_charger = 0;
uint32_t sys_timestamp = 0;
static uint16_t delay_time_10ms = 0;
static void delay_time_ms(int time_ms);
static void delay_ms_timer(uint16_t ms);
static uint8_t cab_app_check_valid_hmi_msg(Cabinet_App *p_ca);
static void cab_app_reset_buffer(Cabinet_App *p_ca);
static void cab_app_process_hmi_write_bss_cmd(Cabinet_App *p_ca, const uint8_t id, const uint32_t timestamp,uint32_t* timestamp1);
static void cab_app_process_hmi_write_cab_cmd(Cabinet_App *p_ca, const uint8_t id, const uint32_t timestamp);
static void cab_app_process_hmi_write_command(Cabinet_App *p_ca, const uint8_t msg_id, const uint32_t timestamp,uint32_t* timestamp1);
static void cab_app_process_hmi_read_command(Cabinet_App *p_ca, const uint8_t msg_id);
static void cab_app_confirm_hmi_cmd(Cabinet_App *p_ca, const uint8_t msg_id, char *buff);
static uint8_t cab_app_get_obj_state(Cabinet_App *p_ca, const uint8_t msg_id);

void cab_app_request_upgrade_fw_bp(Cabinet_App *p_ca, uint8_t cab_id);
void cab_app_active_charge(Cabinet_App *p_ca, uint8_t cab_id) {
	if (p_ca->base.CO_base.sdo_client.status != CO_SDO_RT_idle)
		return;
	p_ca->bss.cabs[cab_id].bp->charge_sw_state = 3;
	p_ca->base.bms_mainswitch_state = BMS_STATE_CHARGING;
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_MAINSWITCH);
	delay_time_ms(10000);
	p_ca->base.sdo_service = SDO_SERVICE_ACTIVE_CHARGER;
}
void cab_app_deactive_charge(Cabinet_App *p_ca, uint8_t cab_id) {
	if (p_ca->base.CO_base.sdo_client.status != CO_SDO_RT_idle)
		return;
	p_ca->bss.cabs[cab_id].bp->charge_sw_state = 0;
	p_ca->base.bms_mainswitch_state = BMS_STATE_DISCHARGING;
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_MAINSWITCH);
	delay_time_ms(10000);
	p_ca->base.sdo_service = SDO_SERVICE_DEACTIVE_CHARGER;
}
void cab_app_delivery_bp(Cabinet_App *p_ca, CABIN_ID cab_id) {
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
	bp_reset_data(p_ca->bss.cabs[cab_id].bp);
	p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
	if (p_ca->bss.cabs[cab_id].op_state == CAB_CELL_ST_INIT)
		return;
	p_ca->bss.cabs[cab_id].op_state = CAB_CELL_ST_EMPTY;
}
void cab_app_sync_bss_data_hmi(Cabinet_App *p_ca) {
	bss_data_serialize(&p_ca->bss, tx_buff);
	uart_sends(&hmi_com, (uint8_t*) tx_buff);
}
void cab_app_sync_bp_data_hmi(Cabinet_App *p_ca, uint8_t cab_id) {
	if ((p_ca->bss.cabs[cab_id].bp->base.con_state != CO_SLAVE_CON_ST_CONNECTED) || (p_ca->bss.cabs[cab_id].bp->vol == 0))
		return;

	bp_data_serialize(p_ca->bss.cabs[cab_id].bp, tx_buff);
	uart_sends(&hmi_com, (uint8_t*) tx_buff);
}
void cab_app_sync_cab_data_hmi(Cabinet_App *p_ca, uint8_t cab_id) {
	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], tx_buff);
	uart_sends(&hmi_com, (uint8_t*) tx_buff);
}
void cab_app_send_msg_to_hmi(Cabinet_App *p_ca) {
	(void) p_ca;
	uart_sends(&hmi_com, (uint8_t*) tx_buff);
	uart_sends(&debug_com, (uint8_t*) tx_buff);

}

void cab_app_process_hmi_command(Cabinet_App *p_ca, const uint32_t timestamp,uint32_t* timestamp1) {

	for (uint8_t i = 0; i < p_ca->hmi_csv.valid_msg_num; i++) {
		switch (p_ca->hmi_csv.cmd_code[i]) {
			case HMI_WRITE:
				cab_app_process_hmi_write_command(p_ca, i, timestamp, timestamp1);
				break;
			case HMI_READ:
				cab_app_process_hmi_read_command(p_ca, i);
				break;
			default:
				p_ca->hmi_csv.obj_state[i] = STATE_FAIL;
				break;
		}
		if (p_ca->hmi_csv.sub_obj[i] != ALL) {
			cab_app_confirm_hmi_cmd(p_ca, i, tx_buff);
			cab_app_send_msg_to_hmi(p_ca);
		}
		p_ca->hmi_csv.cmd_code[i] = p_ca->hmi_csv.id[i] = p_ca->hmi_csv.main_obj[i] = p_ca->hmi_csv.obj_state[i] = p_ca->hmi_csv.sub_obj[i] = 0;
		for (uint8_t j = 0; j < 32; j++) {
			p_ca->hmi_csv.data[j] = '\0';
		}
	}
	p_ca->hmi_csv.valid_msg_num = 0;
}
static void cab_app_process_hmi_write_command(Cabinet_App *p_ca, const uint8_t msg_id, const uint32_t timestamp, uint32_t* timestamp1) {
	switch (p_ca->hmi_csv.main_obj[msg_id]) {
		case BSS_STATION:
			cab_app_process_hmi_write_bss_cmd(p_ca, msg_id, timestamp, timestamp1);
			break;
		case BSS_CABINET:
			cab_app_process_hmi_write_cab_cmd(p_ca, msg_id, timestamp);
			break;
		case BSS_BP:
			break;
		default:
			p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
			break;
	}
}
static void cab_app_process_hmi_read_command(Cabinet_App *p_ca, const uint8_t msg_id) {
	switch (p_ca->hmi_csv.main_obj[msg_id]) {
		case BSS_STATION:
			if (p_ca->hmi_csv.sub_obj[msg_id] == ALL) {
				bss_data_serialize(&p_ca->bss, tx_buff);
				cab_app_send_msg_to_hmi(p_ca);
			} else {
				p_ca->hmi_csv.obj_state[msg_id] = cab_app_get_obj_state(p_ca, msg_id);
				cab_app_confirm_hmi_cmd(p_ca, msg_id, tx_buff);
			}
			break;
		case BSS_CABINET:
			if(p_ca->hmi_csv.id[msg_id] == 'A'){
					bss_all_cabinet_data_serialize(&p_ca->bss, tx_buff);
			}else{
				if (p_ca->hmi_csv.sub_obj[msg_id] == ALL) {
					cab_cell_data_serialize(&p_ca->bss.cabs[p_ca->hmi_csv.id[msg_id]], tx_buff);
					cab_app_send_msg_to_hmi(p_ca);
				} else {
					p_ca->hmi_csv.obj_state[msg_id] = cab_app_get_obj_state(p_ca, msg_id);
					cab_app_confirm_hmi_cmd(p_ca, msg_id, tx_buff);
				}
			}
			break;
		case BSS_BP:
			bp_data_serialize(p_ca->bss.cabs[p_ca->hmi_csv.id[msg_id]].bp, tx_buff);
			cab_app_send_msg_to_hmi(p_ca);
			break;
		default:
			break;
	}
}
static void cab_app_process_hmi_write_bss_cmd(Cabinet_App *p_ca, const uint8_t msg_id, const uint32_t timestamp,uint32_t* timestamp1) {
	uint8_t id = p_ca->hmi_csv.id[msg_id];
	uint8_t state = p_ca->hmi_csv.obj_state[msg_id];
	SUB_OBJS sub_obj = p_ca->hmi_csv.sub_obj[msg_id];
	switch (sub_obj) {
		case BSS_ID_ASSIGN:
			can_master_start_assign_slave((CAN_master*) p_ca, p_ca->base.slaves[state], timestamp);
			reassign_attemp_cnt = 0;
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			break;
		case BSS_AUTHORIZE:
			if (p_ca->base.assign_state == CM_ASSIGN_ST_AUTHORIZING) {
				if (state == STATE_OK) {
					p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
					co_slave_set_con_state(p_ca->base.assigning_slave, CO_SLAVE_CON_ST_CONNECTED);
				} else if (state == STATE_FAIL) {
					p_ca->base.assign_state = CM_ASSIGN_ST_FAIL;
				}
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			} else
				p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
			break;
		case STATE:
			bss_set_state(&p_ca->bss, (BSS_STATE) state);
			if(p_ca->bss.state == BSS_ST_MAINTAIN){
				p_ca->is_main_hmi_shutdown = false;
			}
			p_ca->base.sdo_service = SDO_SERVICE_IDLE;
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
			sw_process(&p_ca->bss.bss_fans[id], (SW_STATE) state);
			if (p_ca->bss.bss_fans[id].state == (SW_STATE) state) {
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			} else
				p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
			break;
		case LAMP:
			bss_set_led_color(&p_ca->bss, (BSS_LED_COLOR) state);
			break;
		case CHARGER:
			sw_process(&p_ca->bss.ac_chargers[id].input_power, (SW_STATE) state);
			if (p_ca->bss.ac_chargers[id].input_power.state == (SW_STATE) state) {
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			} else
				p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
			break;
		case POWER_METER:
			if(state == 1){
				for(uint8_t cmd = 1;cmd < 7;cmd ++){
					selex_bss_app.bss.ac_meter.timeout = *timestamp1 + READ_AC_METTER_TIMEOUT_mS;
					p_ca->slave_com->state = RS485_MASTER_ST_MOBUS;
					UART_set_baudrate_rs485(9600);
					mobus_master_command_serialize(p_ca->slave_com,cmd);
					mobus_master_sends(p_ca->slave_com);
					while(selex_bss_app.bss.ac_meter.finish_read == false);
					uint16_t crc;
					crc = MODBUS_CRC16(p_ca->bss.ac_meter.rx_packet,9);
					if(crc == 0){
						bss_update_ac_meter(&p_ca->bss,cmd);
						p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
					}else{
						p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
					}
					for (int i = 0;i<32;i++){
						p_ca->slave_com->rx_data[i] = 0;
					}
					p_ca->slave_com->rx_index = 0;
					bss_clear_packet(&p_ca->bss);
					UART_set_baudrate_rs485(115200);
					selex_bss_app.bss.ac_meter.rx_index = 0;
					p_ca->slave_com->state = RS485_MASTER_ST_IDLE;
					selex_bss_app.bss.ac_meter.finish_read = false;
				}
			}
			if(state == 0){
				UART_set_baudrate_rs485(115200);
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			}
			break;
		default:
			p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
			break;
	}
}
static void cab_app_process_hmi_write_cab_cmd(Cabinet_App *p_ca, const uint8_t msg_id,const uint32_t timestamp) {
	SUB_OBJS sub_obj = p_ca->hmi_csv.sub_obj[msg_id];
	uint8_t state = p_ca->hmi_csv.obj_state[msg_id];
	uint8_t id = p_ca->hmi_csv.id[msg_id];
	switch (sub_obj) {
		case DOOR:
			if (state == SW_ST_ON) {
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
			if(state == 0){
				cab_app_deactive_charge(p_ca, p_ca->bss.cabs[id].cab_id);
				if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_success && p_ca->base.sdo_service == SDO_SERVICE_DEACTIVE_CHARGER) {
					p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
					p_ca->base.sdo_service = SDO_SERVICE_IDLE;
				} else if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort && p_ca->base.sdo_service == SDO_SERVICE_DEACTIVE_CHARGER) {
					p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
					p_ca->base.sdo_service = SDO_SERVICE_IDLE;
				}
				if (p_ca->bss.cabs[id].bp->state == BP_ST_STANDBY) {
					sw_off(&p_ca->bss.cabs[id].charger);
					p_ca->bss.cabs[id].op_state = CAB_CELL_ST_STANDBY;
					p_ca->bss.ac_chargers[0].charging_cabin = NULL;
					p_ca->bss.ac_chargers[1].charging_cabin = NULL;
					charge_no_cur_timestamp[id] = 0;
					swicth_time_charger = 0;
					p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
				}else {
					p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
				}
			}
			if(state == 1){
				sw_on(&p_ca->bss.cabs[id].charger);
				if(p_ca->bss.cabs[id].charger.state == 1){
					cab_app_active_charge(p_ca, p_ca->bss.cabs[id].cab_id);
					if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_success && p_ca->base.sdo_service == SDO_SERVICE_ACTIVE_CHARGER) {
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
						p_ca->base.sdo_service = SDO_SERVICE_IDLE;
					}
					if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort && p_ca->base.sdo_service == SDO_SERVICE_ACTIVE_CHARGER) {
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
						p_ca->base.sdo_service = SDO_SERVICE_IDLE;
					}
					if (p_ca->bss.cabs[id].bp->state == BP_ST_CHARGING) {
						p_ca->bss.cabs[id].op_state = CAB_CELL_ST_CHARGING;
						p_ca->bss.cabs[id].on_bp_counter[id] = 0;
						p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
					}else{
						p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
					}
				}else{
					p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
				}
			}
			break;
		case OP_STATE:
			if (state == CAB_CELL_ST_INACTIVE) {
				cab_cell_reset(&p_ca->bss.cabs[id]);
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			} else if (state == CAB_CELL_ST_EMPTY) {
				cab_cell_set_op_state(&p_ca->bss.cabs[id], CAB_CELL_ST_EMPTY);
				co_slave_set_con_state(&p_ca->bss.cabs[id].bp->base, CO_SLAVE_CON_ST_DISCONNECT);
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			} else if (state == CAB_CELL_ST_INIT) {
				p_ca->bss.cabs[id].op_state = CAB_CELL_ST_INIT;
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			}
			break;
		case LAMP:
			rs485_master_process_switch_command(p_ca->slave_com, 0, SLAVE_LED, state);
			break;
		case WRITE_SN:
			memcpy(p_ca->base.hmi_xe_sn, p_ca->hmi_csv.data, sizeof(p_ca->hmi_csv.data));
			cab_app_write_bss_sn(p_ca, id);
			p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			break;
		case REBOOT_BP:
			CO_SDO_abort(&p_ca->base.CO_base.sdo_client, CO_SDO_AB_GENERAL);
			int count = 0;
			while (count++ < 5) {
				CO_SDO_reset_status(&p_ca->base.CO_base.sdo_client);
				p_ca->base.sdo_timeout = timestamp + 500;
				p_ca->base.sdo_service = SDO_SERVICE_REQ_BOOT_BMS;
				cab_app_request_upgrade_fw_bp(p_ca, id);
				while(p_ca->base.sdo_finish == false);
				if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_success) {
					p_ca->base.sdo_service = SDO_SERVICE_BOOT_BMS;
					p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
					p_ca->is_main_hmi_shutdown = true;
					p_ca->upgrade_timeout = timestamp + 5000;
					break;
				} else {
					p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
					p_ca->base.sdo_service = SDO_SERVICE_IDLE;
				}
			}
			p_ca->base.sdo_finish = false;
			CO_SDO_reset_status(&p_ca->base.CO_base.sdo_client);
			sw_off(&p_ca->bss.cabs[id].charger);
			if (id == 0 || id == 1 || id == 4 || id == 5 || id == 8 || id == 9 || id == 12 || id == 13 || id == 16) {
				p_ca->bss.ac_chargers[0].charging_cabin = NULL;
			} else {
				p_ca->bss.ac_chargers[1].charging_cabin = NULL;
			}
			break;
		case REBOOT_SUCCESS:
			p_ca->base.sdo_service = SDO_SERVICE_IDLE;
			p_ca->is_main_hmi_shutdown = false;
			if(state == 0){
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			}
			if(state == 1){
				p_ca->hmi_csv.obj_state[msg_id] = STATE_OK;
			}
			break;
		default:
			p_ca->hmi_csv.obj_state[msg_id] = STATE_FAIL;
			break;
	}
}

static uint8_t cab_app_get_obj_state(Cabinet_App *p_ca, const uint8_t msg_id) {
	uint8_t obj_state = 0;
	MAIN_OBJS bss_obj = p_ca->hmi_csv.main_obj[msg_id];
	uint8_t obj_id = p_ca->hmi_csv.id[msg_id];

	switch (p_ca->hmi_csv.sub_obj[msg_id]) {
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
			if (bss_obj == BSS_STATION) {
				obj_state = p_ca->bss.ac_chargers[obj_id].input_power.state;
			} else
				obj_state = p_ca->bss.cabs[obj_id].charger.state;
			break;
		case FAN:
			if (bss_obj == BSS_STATION) {
				obj_state = p_ca->bss.bss_fans[obj_id].state;
			} else
				obj_state = p_ca->bss.cabs[obj_id].cell_fan.state;
			break;
		case LAMP:
			obj_state = p_ca->bss.bss_lamps[obj_id].state;
			break;
		case TEMP:
			if (bss_obj == BSS_STATION) {
				obj_state = p_ca->bss.bss_temps[obj_id];
			} else
				obj_state = p_ca->bss.cabs[obj_id].temp[0];
			break;
		default:
			break;
	}
	return obj_state;
}

static void cab_app_confirm_hmi_cmd(Cabinet_App *p_ca, const uint8_t msg_id, char *buff) {
	*buff++ = ':';
	*buff++ = p_ca->hmi_csv.cmd_code[msg_id];
	*buff++ = ',';
	*buff++ = p_ca->hmi_csv.main_obj[msg_id];
	*buff++ = ',';
	buff += long_to_string(p_ca->hmi_csv.id[msg_id], buff);
	*buff++ = ',';
	*buff++ = p_ca->hmi_csv.sub_obj[msg_id];
	*buff++ = ',';
	if (p_ca->hmi_csv.cmd_code[msg_id] == HMI_WRITE) {
		*buff++ = p_ca->hmi_csv.obj_state[msg_id];
		*buff++ = ',';
		if (p_ca->hmi_csv.data[0] == '\0') {
			*buff++ = '0';
		} else {
			for (uint8_t i = 0; *(p_ca->hmi_csv.data + i) != '\0'; i++) {

				*buff++ = *(p_ca->hmi_csv.data + i);
			}
		}
	} else
		buff += long_to_string(p_ca->hmi_csv.obj_state[msg_id], buff);
	*buff++ = '*';
	*buff++ = '\n';
	*buff++ = '\0';
}

static void cab_app_reset_buffer(Cabinet_App *p_ca) {
	p_ca->rx_index = 0;
	for (uint8_t i = 0; i < 32; i++) {
		p_ca->rx_data[i] = 0;
	}
	p_ca->is_new_msg = 0;
}

static int16_t cab_app_search_char(char *buff, uint16_t start, uint16_t stop, char find_char) {
	for (uint16_t i = start; i < stop; i++) {
		if (buff[i] == find_char)
			return i;
	}
	return -1;
}

void cab_app_check_buffer(Cabinet_App *p_ca) {
	uint8_t *buff = (uint8_t*) p_ca->rx_data;
	int16_t start = -1;
	int16_t stop = -1;
	do {
		start = cab_app_search_char((char*) buff, stop + 1, p_ca->rx_index, ':');
		if (start == -1)
			break;
		stop = cab_app_search_char((char*) buff, stop + 2, p_ca->rx_index, '*');
		if (stop <= start)
			break;
		uint8_t *token = &buff[start + 1];
		token = (uint8_t*) strtok((char*) token, ",");
		p_ca->hmi_csv.cmd_code[p_ca->hmi_csv.valid_msg_num] = *token;
		token = (uint8_t*) strtok(NULL, ",");
		p_ca->hmi_csv.main_obj[p_ca->hmi_csv.valid_msg_num] = *token;
		token = (uint8_t*) strtok(NULL, ",");
		if(*token != 'A'){
			p_ca->hmi_csv.id[p_ca->hmi_csv.valid_msg_num] = atoi((char*) token);
		}else{
			p_ca->hmi_csv.id[p_ca->hmi_csv.valid_msg_num] = *token;
		}

		token = (uint8_t*) strtok(NULL, ",");
		p_ca->hmi_csv.sub_obj[p_ca->hmi_csv.valid_msg_num] = *token;
		if (*token != 'V') {
			if(*token != 'B'){
				token = (uint8_t*) strtok(NULL, "*");
				if ((*token != STATE_OK) && (*token != STATE_FAIL)) {
					p_ca->hmi_csv.obj_state[p_ca->hmi_csv.valid_msg_num] = atoi((char*) token);
				} else {
					p_ca->hmi_csv.obj_state[p_ca->hmi_csv.valid_msg_num] = *token;
				}
			} else {
				token = (uint8_t*) strtok(NULL, "*");
				memset(p_ca->hmi_csv.firmware, 0, sizeof(p_ca->hmi_csv.firmware));
				memcpy(p_ca->hmi_csv.firmware, token, strlen((const char*) token));
			}
		} else {
			token = (uint8_t*) strtok(NULL, "*");
			memcpy(p_ca->hmi_csv.data, token, strlen((const char*) token));
		}
		p_ca->hmi_csv.valid_msg_num++;
	} while ((start >= 0) && (stop > start));
	p_ca->is_new_msg = 0;
	cab_app_reset_buffer(p_ca);
}

void cab_app_update_charge(Cabinet_App *p_ca) {
	for (uint8_t id = 0; id < p_ca->bss.charger_num; id++) {
		if (p_ca->bss.ac_chargers[id].charging_cabin != NULL) {
			if ((p_ca->bss.ac_chargers[id].charging_cabin->bp->vol >= BP_STOP_CHARGER_THRESHOLD) || (charge_no_cur_timestamp[id] >= CHARGER_NO_CURRENT_TIMEOUT)) {
				if ((p_ca->bss.ac_chargers[id].charging_cabin->bp->vol >= BP_OVER_CHARGE_THRESHOLD) && (p_ca->bss.ac_chargers[id].charging_cabin->bp->cur < CUR_STOP_CHARGER_THRESHOLD)){
					cab_app_deactive_charge(p_ca, p_ca->bss.ac_chargers[id].charging_cabin->cab_id);
					if (p_ca->bss.ac_chargers[id].charging_cabin->bp->state == BP_ST_STANDBY) {
						sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
						p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_STANDBY;
						p_ca->bss.ac_chargers[id].charging_cabin = NULL;
						charge_no_cur_timestamp[id] = 0;
					}
					if ((p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort) && (p_ca->base.sdo_service == SDO_SERVICE_DEACTIVE_CHARGER)) {
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
						p_ca->base.sdo_service = SDO_SERVICE_IDLE;
					}

				}
				if((charge_no_cur_timestamp[id] >= CHARGER_NO_CURRENT_TIMEOUT))
				{
					cab_app_deactive_charge(p_ca, p_ca->bss.ac_chargers[id].charging_cabin->cab_id);
					if (p_ca->bss.ac_chargers[id].charging_cabin->bp->state == BP_ST_STANDBY) {
						sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
						p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_STANDBY;
						p_ca->bss.ac_chargers[id].charging_cabin = NULL;
						charge_no_cur_timestamp[id] = 0;
						swicth_time_charger = 0;
					}
					if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort && p_ca->base.sdo_service == SDO_SERVICE_DEACTIVE_CHARGER) {
						p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
						p_ca->base.sdo_service = SDO_SERVICE_IDLE;
					}
				}
			}
			else if (p_ca->bss.ac_chargers[id].charging_cabin->bp->base.con_state == CO_SLAVE_CON_ST_DISCONNECT) {
				sw_off(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
				p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_EMPTY;
				p_ca->bss.ac_chargers[id].charging_cabin = NULL;
				charge_no_cur_timestamp[id] = 0;
			}
			else {
				if (cab_cell_get_op_state(p_ca->bss.ac_chargers[id].charging_cabin) == CAB_CELL_ST_CHARGING) {
					if (p_ca->bss.ac_chargers[id].charging_cabin->bp->cur < 100)
						charge_no_cur_timestamp[id] += 10;
					else {
						charge_no_cur_timestamp[id] = 0;
					}
					continue;
				}
				cab_app_active_charge(p_ca, p_ca->bss.ac_chargers[id].charging_cabin->cab_id);
				p_ca->bss.ac_chargers[id].charging_cabin->on_bp_counter[id]++;
				if (p_ca->bss.ac_chargers[id].charging_cabin->bp->state == BP_ST_CHARGING) {
					p_ca->bss.ac_chargers[id].charging_cabin->op_state = CAB_CELL_ST_CHARGING;
					p_ca->bss.ac_chargers[id].charging_cabin->on_bp_counter[id] = 0;
				}
				if (p_ca->base.CO_base.sdo_client.status == CO_SDO_RT_abort && p_ca->base.sdo_service == SDO_SERVICE_ACTIVE_CHARGER) {
					p_ca->base.CO_base.sdo_client.status = CO_SDO_RT_idle;
					p_ca->base.sdo_service = SDO_SERVICE_IDLE;
				}
			}
		} else {
			Cabinet *cab = bss_get_cab_need_charge(&p_ca->bss, id);
			if (cab != NULL) {
				delay_time_10ms++;
				if (delay_time_10ms > 100) {
					delay_time_10ms = 0;
					p_ca->bss.ac_chargers[id].charging_cabin = cab;
					sw_on(&p_ca->bss.ac_chargers[id].charging_cabin->charger);
				}
			}
		}
	}
}
void cab_app_update_connected_cab_state(Cabinet_App *p_app) {
	for (uint8_t id = 0; id < p_app->bss.cab_num; id++) {
//		if (p_app->bss.cabs[id].bp->base.con_state == CO_SLAVE_CON_ST_CONNECTED) {
			if ((p_app->bss.cabs[id].op_state == CAB_CELL_ST_STANDBY) || (p_app->bss.cabs[id].op_state == CAB_CELL_ST_CHARGING) ){
				if (p_app->bss.cabs[id].bp->base.inactive_time_ms > sys_timestamp)
					continue;
				cab_cell_reset(&p_app->bss.cabs[id]);
				for (uint8_t i = 0; i < p_app->bss.charger_num; i++) {
					if (p_app->bss.ac_chargers[i].charging_cabin != &p_app->bss.cabs[id])
						continue;
					sw_off(&p_app->bss.ac_chargers[i].charging_cabin->charger);
					p_app->bss.ac_chargers[id].charging_cabin = NULL;
				}
			}
//		}
	}
}
void cab_app_write_bss_sn(Cabinet_App *p_ca, uint8_t cab_id){
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_MATED_DEV);
}
void cabinet_app_data_log_serialize(Cabinet_App *p_ca, char *buff) {
	*buff++ = ':';
	if (p_ca->hmi_csv.data[0] == '\0') {
		*buff++ = '0';
	} else {
		for (uint8_t i = 0; *(p_ca->hmi_csv.data + i) != '\0'; i++) {

			*buff++ = *(p_ca->hmi_csv.data + i);
		}
	}
	*buff++ = ',';
	if (p_ca->base.hmi_xe_sn[0] == '\0') {
		*buff++ = '0';
	} else {
		for (uint8_t i = 0; *(p_ca->base.hmi_xe_sn + i) != '\0'; i++) {

			*buff++ = *(p_ca->base.hmi_xe_sn + i);
		}
	}
	*buff++ = ',';
	if(p_ca->hmi_csv.firmware[0] == '\0'){
		*buff++ = '0';
	}else{
		for (uint16_t i = 0; *(p_ca->hmi_csv.firmware + i) != '\0'; i++){
			*buff++ = *(p_ca->hmi_csv.firmware + i);
		}
	}
	*buff++ = ',';
	buff += long_to_string(p_ca->base.write, buff);
	*buff++ = ',';
	buff += long_to_string(p_ca->base.write_success, buff);
	*buff++ = ',';
	buff += long_to_string(p_ca->base.write_abort, buff);
	*buff++ = ',';
	buff += long_to_string(p_ca->base.read, buff);
	*buff++ = ',';
	buff += long_to_string(p_ca->base.read_success, buff);
	*buff++ = ',';
	buff += long_to_string(p_ca->base.read_abort, buff);
	*buff++ = '*';
	*buff++ = '\n';
	*buff++ = '\0';
}
void cab_app_send_data_log(Cabinet_App *p_ca) {
	(void) p_ca;
	cabinet_app_data_log_serialize(p_ca, data_log);
	uart_sends(&debug_com, (uint8_t*) data_log);
}
static void delay_time_ms(int time_ms){
	int i,j;
	for(i = 0;i<time_ms;i++){
		for(j = 0; j < 40;j++);
	}
}
static void delay_ms_timer(uint16_t ms)
{
    __HAL_TIM_SET_COUNTER(&io_scan_timer,0);  // set the counter value a 0
    while (__HAL_TIM_GET_COUNTER(&io_scan_timer) < ms);  // wait for the counter to reach the us input in the parameter
}

void cab_app_request_upgrade_fw_bp(Cabinet_App *p_ca, uint8_t cab_id) {
	p_ca->base.bms_new_fw_req = 1;
	can_master_write_bms_object(&p_ca->base, cab_id, BMS_NEW_FW_REQ);
}
