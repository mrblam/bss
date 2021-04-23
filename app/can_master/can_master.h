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
#include "can_hal.h"

#define SDO_RX_BUFFER_SIZE                 (32UL)
typedef struct CO_SDO_SERVER_t CO_SDO_SERVER;

typedef enum SDO_STATE_t{
        SDO_ST_IDLE=        0,
        SDO_ST_SENT=			1,
		SDO_ST_SUCCESS=	2,
		SDO_ST_FAIL=			3
}SDO_STATE;
struct CO_SDO_SERVER_t{
		uint8_t rx_buffer[SDO_RX_BUFFER_SIZE];
        uint32_t timeout;
        SDO_STATE state;
        uint16_t receive_index;
        uint32_t tx_address;
        uint32_t rx_address;
        uint32_t object_mux;
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
	uint32_t slave_num;
	ASSIGN_STATE	assign_state;
	CO_Slave**		slaves;
	CO_Slave* assigning_slave;
	uint16_t 		time_stamp;
	CO_SDO_SERVER 	sdo_server;
	CAN_Hw* p_hw;
	void (*slave_select)(const CAN_master* p_cm,const uint32_t slave_id);
};

static inline void can_master_slave_select(const CAN_master* p_cm, const uint32_t id){
	p_cm->slave_select(p_cm,id);
}
void can_master_init(CO_Slave** slaves,CAN_Hw* p_hw,CAN_master* p_cm);
void can_master_process(CAN_master* p_cm,const uint32_t timestamp);
void can_master_process_sdo(CAN_master* p_cm,const uint32_t timestamp);
void can_master_start_assign_node_id(CAN_master* p_cm,const uint32_t slave_id);
void can_master_on_slave_assign_request(CAN_master* p_cm);

void can_master_send_id_msg(CAN_master* p_cm, uint8_t cab_id);
void can_master_request_read_bp_sn(CAN_master* p_cm, uint8_t cab_id);
void can_master_send_sync_request(CAN_master* p_cm,const uint32_t timestamp);

static inline SDO_STATE sdo_server_get_state(const CO_SDO_SERVER* const p_svr){
        return p_svr->state;
}

static inline void sdo_server_set_state(CO_SDO_SERVER* p_svr,const SDO_STATE state){
        p_svr->state=state;
}

#endif /* APP_CAN_MASTER_CAN_MASTER_H_ */
