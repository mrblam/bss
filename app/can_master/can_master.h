/*
 * can_master.h
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#ifndef APP_CAN_MASTER_CAN_MASTER_H_
#define APP_CAN_MASTER_CAN_MASTER_H_

#include "stdlib.h"
#include "stdint.h"
#include "linked_list.h"

typedef struct CAN_master_t CAN_master;

typedef enum ASSIGN_STATE{
	ASSIGN_ST_OK = 0,
	ASSIGN_ST_ERROR = 1,
} ASSIGN_STATE;

struct CAN_master_t{
	SLAVE_ID		slave_id;
	void			(*active_node_id_pin)(uint8_t node_id_pin);
	void			(*deactive_node_id_pin)(uint8_t node_id_pin);
	ASSIGN_STATE	assign_state;
	List*			empty_slave_list;
};

CAN_master* can_master_construct(void);
void can_master_init(CAN_master* p_cm);

void can_master_active_node_id_pin(CAN_master* p_cm, uint8_t node_id_pin);
void can_master_deactive_node_id_pin(CAN_master* p_cm, uint8_t node_id_pin);
void can_master_response_id_assign_req(CAN_master* p_cm);
uint8_t can_master_select_empty_slave(CAN_master* p_cm);
void can_master_send_id_msg(CAN_master* p_cm);
ASSIGN_STATE can_master_wait_slave_confirm(CAN_master* p_cm);
void can_master_reset_assign_state(CAN_master* p_cm);
void can_master_update_slave_list(CAN_master* p_cm);


#endif /* APP_CAN_MASTER_CAN_MASTER_H_ */
