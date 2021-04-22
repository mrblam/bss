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
#include "board.h"
#include "app_config.h"
#include "CO.h"

typedef struct CAN_master_t CAN_master;

typedef enum ASSIGN_STATE{
	ASSIGN_ST_INACTIVE,
	ASSIGN_ST_START,
	ASSIGN_ST_SELECT_SLAVE,
	ASSIGN_ST_SEND_ID,
	ASSIGN_ST_WAIT_CONFIRM,
	ASSIGN_ST_DONE,
	ASSIGN_ST_AUTHENTIC
} ASSIGN_STATE;

struct CAN_master_t{
	void			(*active_node_id_pin)(uint8_t node_id_pin);
	void			(*deactive_node_id_pin)(uint8_t node_id_pin);
	ASSIGN_STATE	assign_state;
	List*			empty_slave_list;
	TIMING_STATE	timing_state;
	uint16_t 		time_stamp;
};

void can_master_init(CAN_master* p_cm);
void can_master_accept_assign_request(__attribute__((unused)) CAN_master* p_cm);
uint8_t can_master_select_slave(CAN_master* p_cm);
void can_master_send_id_msg(CAN_master* p_cm, uint8_t cab_id);
void can_master_update_slave_list(CAN_master* p_cm);
void can_master_request_read_bp_sn(CAN_master* p_cm, uint8_t cab_id);

static inline void can_master_deactive_node_id_pin(CAN_master* p_cm, uint8_t cab_id){
	p_cm->deactive_node_id_pin(cab_id);
}

static inline void can_master_active_node_id_pin(CAN_master* p_cm, uint8_t cab_id){
	p_cm->active_node_id_pin(cab_id);
}

void can_master_active_timing_state(CAN_master* p_cm, uint16_t time_ms);
void can_master_deactive_timing_state(CAN_master* p_cm);


#endif /* APP_CAN_MASTER_CAN_MASTER_H_ */
