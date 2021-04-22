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

typedef struct CO_SDO_SERVER_t CO_SDO_SERVER;

typedef enum SDO_STATE_t{
        SDO_ST_IDLE=        0,
        SDO_ST_SENT
}SDO_STATE;
struct CO_SDO_SERVER_t{
        uint32_t timeout;
        SDO_STATE state;
        uint16_t receive_index;
        uint32_t server_address;
};

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
	CO_Slave_List slaves;
	CO_SDO_SERVER sdo_server;
	CO_Slave* assigning_slave;
};

static inline CO_Slave* can_master_get_unassign_slave(const CAN_master* const p_cm){
        CO_Slave_List* slaves=&p_cm->slaves;

        while(slaves!=NULL){
                if(slaves->data->con_state==CO_SLAVE_CON_ST_UNASSIGNED){
                       return slaves->data;
                }
                slaves=slaves->next;
        }
        return NULL;
}

void can_master_process(CAN_master* p_cm,const uint32_t timestamp);

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
void can_master_send_sync_request(CAN_master* p_cm,const uint32_t timestamp);

static inline SDO_STATE sdo_server_get_state(const CO_SDO_SERVER* const p_svr){
        return p_svr->state;
}

static inline void sdo_server_set_state(CO_SDO_SERVER* p_svr,const SDO_STATE state){
        p_svr->state=state;
}


#endif /* APP_CAN_MASTER_CAN_MASTER_H_ */
