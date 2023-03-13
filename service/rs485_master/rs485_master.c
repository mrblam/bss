/*
 * rs485_master.c
 *
 *  Created on: May 8, 2021
 *      Author: KhanhDinh
 */

#include "rs485_master.h"

static void rs485_master_command_serialize(RS485_Master* p_485m);
static void rs485_master_sync_data_serialize(RS485_Master* p_485m);
static void rs485_master_sends(RS485_Master* p_485m);
static uint8_t rs485_master_check_valid_msg(RS485_Master* p_485m);

RS485_Master* rs485_master_construct(void){
	RS485_Master* p_485m = (RS485_Master*)malloc(sizeof(RS485_Master));
	while(p_485m == NULL);
	return p_485m;
}

void rs485_master_init(RS485_Master* p_485m, Max485_Hw* p_hw){
	p_485m->state = RS485_MASTER_ST_IDLE;
	p_485m->p_hw = p_hw;
	p_485m->is_new_msg = 0;
	rs485_master_reset_buffer(p_485m);
	p_485m->start_msg_index = 0;
	p_485m->csv.id = p_485m->csv.obj = 0;
}

void rs485_master_reset_buffer(RS485_Master* p_485m){
	p_485m->rx_index = 0;
	for(uint8_t i = 0; i < 32; i++){
		p_485m->rx_data[i] = 0;
		p_485m->tx_data[i] = 0;
	}
}

void rs485_master_set_csv_data(RS485_Master* p_485m, const uint8_t slave_id,
		const char obj, const uint8_t state){
	p_485m->csv.id = slave_id;
	p_485m->csv.obj = obj;
	p_485m->csv.state = state;
}

void rs485_master_update_state(RS485_Master* p_485m, const uint32_t timestamp){
	switch(p_485m->state){
	case RS485_MASTER_ST_IDLE:
		break;
	case RS485_MASTER_ST_SEND_CMD:
		rs485_master_command_serialize(p_485m);
		rs485_master_sends(p_485m);
		p_485m->timeout = timestamp + 200;
		p_485m->state = RS485_MASTER_ST_WAIT_CONFIRM;
		break;
	case RS485_MASTER_ST_WAIT_CONFIRM:
		if(p_485m->is_new_msg){
			if(rs485_master_check_valid_msg(p_485m)){
				p_485m->parse_slave_msg_handle(p_485m);
				p_485m->is_new_msg = 0;
				return;
			}
		}
		if(p_485m->timeout < timestamp){
			p_485m->state = RS485_MASTER_ST_FAIL;
			return;
		}
		if((p_485m->timeout - timestamp) == 100){
			if(p_485m->csv.obj){
				rs485_master_command_serialize(p_485m);
			}
			else rs485_master_sync_data_serialize(p_485m);
			rs485_master_sends(p_485m);
		}
		break;
	case RS485_MASTER_ST_SEND_SYNC:
		p_485m->csv.obj = p_485m->csv.state = 0;
		rs485_master_sync_data_serialize(p_485m);
		rs485_master_sends(p_485m);
		p_485m->timeout = timestamp + 200;
		p_485m->state = RS485_MASTER_ST_WAIT_CONFIRM;
		break;
	case RS485_MASTER_ST_SUCCESS:
	case RS485_MASTER_ST_FAIL:
	default:
		break;
	}
}

static void rs485_master_sends(RS485_Master* p_485m){
	p_485m->set_transmit_mode(p_485m);
	p_485m->p_hw->sends(p_485m->p_hw, (char*)p_485m->tx_data);
	p_485m->set_receive_mode(p_485m);
}

static void rs485_master_command_serialize(RS485_Master* p_485m){
	char* buff = (char*)p_485m->tx_data;
	*buff++=':';
	buff+=long_to_string(p_485m->csv.id, buff);
	*buff++=',';
	*buff++='W';
	*buff++=',';
	*buff++=p_485m->csv.obj;
	*buff++=',';
	buff+=long_to_string(p_485m->csv.state, buff);
	*buff++='*';
	*buff++='\0';
}

static void rs485_master_sync_data_serialize(RS485_Master* p_485m){
	char* buff = (char*)p_485m->tx_data;
	*buff++=':';
	buff+=long_to_string(p_485m->csv.id, buff);
	*buff++=',';
	*buff++='R';
	*buff++='*';
	*buff++='\0';
}

static uint8_t rs485_master_check_valid_msg(RS485_Master* p_485m){
	uint8_t* buff = (uint8_t*)p_485m->rx_data;
	buff += p_485m->rx_index-1;
	while((*buff != '*') || (*buff != '\0')){
		if(*buff == '*'){
			*buff = '\0';
			buff--;
			while((*buff != ':') || (*buff != '\0')){
				if(*buff == ':'){
					p_485m->start_msg_index = ++buff;
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

void rs485_master_process_switch_command(RS485_Master* p_485m, uint8_t id, SLAVE_OBJECTS obj, SLAVE_OBJECT_STATE state){
	rs485_master_set_csv_data(p_485m, id, obj, state);
	if(p_485m->state == RS485_MASTER_ST_IDLE){
		p_485m->state = RS485_MASTER_ST_SEND_CMD;
	}
	while((p_485m->state == RS485_MASTER_ST_SEND_CMD) ||
			(p_485m->state == RS485_MASTER_ST_SEND_SYNC) ||
			(p_485m->state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(p_485m);
	p_485m->state = RS485_MASTER_ST_IDLE;
}

void rs485_master_process_sync_data(RS485_Master* p_485m, uint8_t id){
	rs485_master_set_csv_data(p_485m, id, 0, 0);
	if(p_485m->state == RS485_MASTER_ST_IDLE){
		p_485m->state = RS485_MASTER_ST_SEND_SYNC;
	}
	while((p_485m->state == RS485_MASTER_ST_SEND_CMD) ||
			(p_485m->state == RS485_MASTER_ST_SEND_SYNC) ||
			(p_485m->state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(p_485m);
	p_485m->state = RS485_MASTER_ST_IDLE;
}

