/*
 * can_master.h
 *
 *  Created on: Mar 25, 2021
 *      Author: KhanhDinh
 */

#ifndef APP_CAN_MASTER_CAN_MASTER_H_
#define APP_CAN_MASTER_CAN_MASTER_H_

#include "stdlib.h"
#include "stdint.h"
#include "linked_list.h"

#define	NODE_MASTER	0x03
#define SLAVE_NUMBER	15

typedef enum SLAVE_ID{
	SLAVE1 = 0x04,
	SLAVE2 = 0x05,
	SLAVE3 = 0x06,
	SLAVE4 = 0x07,
	SLAVE5 = 0x08,
	SLAVE6 = 0x09,
	SLAVE7 = 0x0a,
	SLAVE8 = 0x0b,
	SLAVE9 = 0x0c,
	SLAVE10 = 0x0d,
	SLAVE11 = 0x0e,
	SLAVE12 = 0x0f,
	SLAVE13 = 0x10,
	SLAVE14 = 0x11,
	SLAVE15 = 0x12
} SLAVE_ID;

typedef enum SLAVE_STATE{
	SLAVE_OK = 0,
	SLAVE_ERROR = 1,
} SLAVE_STATE;

typedef struct CAN_Master {
	LIST* empty_slave;
	LIST* busy_slave;
	uint8_t slave_number;
	uint8_t slave_id;
} CAN_Master;

CAN_Master* cm_construct(void);
void cm_init(CAN_Master* p_cm);
void cm_respone_assign_request(CAN_Master* p_cm);
void cm_send_assign_msg(CAN_Master* p_cm);
SLAVE_STATE cm_select_slave(CAN_Master* p_cm, LIST* p_list);
void cm_deselect_slave(CAN_Master* p_cm, uint8_t slave_id);
SLAVE_STATE cm_wait_slave_confirm(CAN_Master* p_cm);
void cm_update_slave_data(CAN_Master* p_cm);

#endif /* APP_CAN_MASTER_CAN_MASTER_H_ */
