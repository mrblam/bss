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
	RS485_MASTER_ST_SEND_CMD,
	RS485_MASTER_ST_WAIT_CONFIRM,
	RS485_MASTER_ST_SEND_SYNC,
	RS485_MASTER_ST_SUCCESS,
	RS485_MASTER_ST_FAIL,
	RS485_MASTER_ST_MOBUS
} RS485_MASTER_STATE;

typedef enum COMMAND_CODE{
	MASTER_WRITE 	= 'W',
	MASTER_READ		= 'R'
}COMMAND_CODE;

typedef enum SLAVE_OBJECTS{
	SLAVE_DOOR 		= 'D',
	SLAVE_FAN 		= 'F',
	SLAVE_NODE_ID 	= 'N',
	SLAVE_CHARGER	= 'C',
	SLAVE_STATE		= 'S',
	SLAVE_LED		= 'L'
}SLAVE_OBJECTS;

typedef enum SLAVE_OBJECT_STATE{
	DEACTIVE	= 0,
	ACTIVE 		= 1,
	FAIL		= 2
} SLAVE_OBJECT_STATE;

typedef struct RS485_csv_data_t RS485_csv_data;
struct RS485_csv_data_t{
	uint8_t	id;
	char	obj;
	uint8_t state;
};

typedef struct RS485_Master_t RS485_Master;
typedef void (*RS485_act)(RS485_Master* p_485m);
struct RS485_Master_t{
	RS485_MASTER_STATE	state;
	Max485_Hw* 			p_hw;
	uint32_t 			timeout;
	uint8_t 			is_new_msg;
	uint8_t				tx_data[32];
	uint8_t 			rx_index;
	uint8_t				rx_data[32];
	uint8_t*			start_msg_index;
	RS485_csv_data		csv;
	RS485_act			set_transmit_mode;
	RS485_act			set_receive_mode;
	RS485_act			parse_slave_msg_handle;
};

RS485_Master* rs485_master_construct(void);
void rs485_master_init(RS485_Master* p_485m, Max485_Hw* p_hw);
void rs485_master_update_state(RS485_Master* p_485m, const uint32_t timestamp);
void rs485_master_set_csv_data(RS485_Master* p_485m, const uint8_t slave_id, const char obj, const uint8_t state);
void rs485_master_reset_buffer(RS485_Master* p_485m);
void rs485_master_process_switch_command(RS485_Master* p_485m, uint8_t id, SLAVE_OBJECTS obj, SLAVE_OBJECT_STATE state);
void rs485_master_process_sync_data(RS485_Master* p_485m, uint8_t id);
void rs485_master_log(RS485_Master* p_485m,int callback,int can);
void rs485_send_log(RS485_Master* p_485m);
void mobus_master_command_serialize(RS485_Master* p_485m,uint8_t cmd);
void mobus_master_sends(RS485_Master* p_485m);
static inline RS485_MASTER_STATE rs485_master_get_state(const RS485_Master* p_485m){
	return p_485m->state;
}

static inline void rs485_master_set_state(RS485_Master* p_485m, const RS485_MASTER_STATE state){
	p_485m->state = state;
}

#endif /* SERVICE_RS485_MASTER_RS485_MASTER_H_ */
