/*
 * rs485_master.h
 *
 *  Created on: May 8, 2021
 *      Author: KhanhDinh
 */

#ifndef SERVICE_RS485_MASTER_RS485_MASTER_H_
#define SERVICE_RS485_MASTER_RS485_MASTER_H_

#include "stdlib.h"
#include "stdint.h"
#include "max485_hal.h"
#include "string_util.h"
#include "app_config.h"
#include "string.h"

typedef enum RS485_MASTER_STATE{
	RS485_MASTER_ST_IDLE,
	RS485_MASTER_ST_WAIT_CONFIRM,
	RS485_MASTER_ST_SEND_SYNC,
	RS485_MASTER_ST_SUCCESS,
	RS485_MASTER_ST_FAIL,
} RS485_MASTER_STATE;

typedef struct RS485_csv_data_t RS485_csv_data;
struct RS485_csv_data_t{
	uint8_t	id;
	char 	cc;
	char	obj;
};

typedef struct RS485_Master_t RS485_Master;
typedef struct RS485_Slave_t RS485_Slave;
typedef void (*RS485_act)(RS485_Master* p_485m);
struct RS485_Master_t{
	RS485_MASTER_STATE	state;
	Max485_Hw* 			p_hw;
	uint32_t 			timeout;
	uint8_t				slave_num;
	RS485_Slave**		slaves;
	uint8_t 			is_new_msg;
	uint8_t				tx_data[32];
	uint8_t 			rx_index;
	uint8_t				rx_data[32];
	uint8_t*			start_msg_index;
	RS485_csv_data		csv;
	RS485_act			set_transmit_mode;
	RS485_act			set_receive_mode;
	RS485_act			command_serialize;
	RS485_act			sync_data_serialize;
	RS485_act			parse_sync_msg_handle;
};



void rs485_master_init(RS485_Master* p_485m, RS485_Slave** slaves,
		uint8_t slave_num, Max485_Hw* p_hw);

void rs485_master_process(RS485_Master* p_485m, const uint32_t timestamp);
void rs485_master_sync_data(RS485_Master* p_485m, const uint32_t timestamp);
void rs485_master_active_switch(RS485_Master* p_485m, const uint32_t timestamp);
void rs485_master_deactive_switch(RS485_Master* p_485m, const uint32_t timestamp);
void rs485_master_process_switch_command(RS485_Master* p_485m, const uint8_t slave_id,
		const SLAVE_OBJECTS obj, const SLAVE_OBJECT_STATE state, const uint32_t timestamp);
uint8_t rs485_master_check_valid_msg(RS485_Master* p_485m);


static inline void r485_master_set_transmit_mode(RS485_Master* p_485m){
	p_485m->set_transmit_mode(p_485m);
}

static inline void rs485_master_set_receive_mode(RS485_Master* p_485m){
	p_485m->set_receive_mode(p_485m);
}

static inline RS485_MASTER_STATE rs485_master_get_state(const RS485_Master* p_485m){
	return p_485m->state;
}

static inline void rs485_master_set_state(RS485_Master* p_485m, const RS485_MASTER_STATE state){
	p_485m->state = state;
}

#endif /* SERVICE_RS485_MASTER_RS485_MASTER_H_ */
