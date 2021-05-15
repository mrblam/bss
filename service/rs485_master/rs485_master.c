/*
 * rs485_master.c
 *
 *  Created on: May 8, 2021
 *      Author: KhanhDinh
 */

#include "rs485_master.h"

static void rs485_master_switch_command_serialize(RS485_Master* p_485m, const uint8_t slave_id,
		const uint8_t obj, const uint8_t state);
static void rs485_master_sync_data_serialize(RS485_Master* p_485m, const uint8_t slave_id);
static void rs485_master_parse_msg(RS485_Master* p_485m);

RS485_Master* rs485_master_construct(void){
	RS485_Master* p_485m = (RS485_Master*)malloc(sizeof(RS485_Master));
	while(p_485m == NULL);
	return p_485m;
}

void rs485_master_init(RS485_Master* p_485m, RS485_Slave** slaves,
		uint8_t slave_num, Max485_Hw* p_hw){
	p_485m->p_hw = p_hw;
	p_485m->slave_num = slave_num;
	p_485m->slaves = slaves;
	p_485m->is_new_msg = 0;
	p_485m->state = RS485_MASTER_ST_IDLE;
}

static void rs485_master_send_sync(RS485_Master* p_485m){
	static uint8_t slave_id = 0;
	rs485_master_sync_data_serialize(p_485m, slave_id);
	cabinet_485_hw.sends(&cabinet_485_hw, (char*)p_485m->tx_data);
	slave_id++;
}

void rs485_master_process_switch_command(RS485_Master* p_485m, const uint8_t slave_id,
		const SLAVE_OBJECTS obj, const SLAVE_OBJECT_STATE state, const uint32_t timestamp){

	switch(p_485m->state){
	case RS485_MASTER_ST_IDLE:
		p_485m->csv.id = slave_id;
		p_485m->csv.obj = obj;
		rs485_master_switch_command_serialize(p_485m, slave_id, obj, state);
		p_485m->set_transmit_mode(p_485m);
		cabinet_485_hw.sends(&cabinet_485_hw, (char*)p_485m->tx_data);
		p_485m->set_receive_mode(p_485m);
		p_485m->timeout = timestamp + 200;
		p_485m->state = RS485_MASTER_ST_WAIT_CONFIRM;
		break;
	case RS485_MASTER_ST_WAIT_CONFIRM:
		if(p_485m->is_new_msg){
			p_485m->parse_sync_msg_handle(p_485m);
			p_485m->is_new_msg = 0;
		}
		if(p_485m->timeout < timestamp){
			p_485m->state = RS485_MASTER_ST_FAIL;
		}
		break;
	case RS485_MASTER_ST_SEND_SYNC:
		rs485_master_sync_data_serialize(p_485m, p_485m->csv.id);
		p_485m->set_transmit_mode(p_485m);
		cabinet_485_hw.sends(&cabinet_485_hw, (char*)p_485m->tx_data);
		p_485m->set_receive_mode(p_485m);
		p_485m->timeout = timestamp + 200;
		p_485m->state = RS485_MASTER_ST_WAIT_CONFIRM;
		break;
	case RS485_MASTER_ST_SUCCESS:
		break;
	default:
		break;
	}
}

static void rs485_master_switch_command_serialize(RS485_Master* p_485m, const uint8_t slave_id,
		const uint8_t obj, const uint8_t state){
	char* buff = (char*)p_485m->tx_data;
	*buff++=':';
	buff+=long_to_string(slave_id, buff);
	*buff++=',';
	*buff++='W';
	*buff++=',';
	*buff++=(char)obj;
	*buff++=',';
	buff+=long_to_string(state, buff);
	*buff++='*';
	*buff++='\0';
}

static void rs485_master_sync_data_serialize(RS485_Master* p_485m, const uint8_t slave_id){
	char* buff = (char*)p_485m->tx_data;
	*buff++=':';
	buff+=long_to_string(slave_id, buff);
	*buff++=',';
	*buff++='R';
	*buff++='*';
	*buff++='\0';
}

uint8_t rs485_master_check_valid_msg(RS485_Master* p_485m){
	uint8_t* buff = (uint8_t*)p_485m->rx_data;
	buff += p_485m->rx_index-1;
	while((*buff != '*') || (*buff != '\0')){
		if(*buff == '*'){
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



