/*
 * can_master.h
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#ifndef SERVICE_CAN_MASTER_CAN_MASTER_H_
#define SERVICE_CAN_MASTER_CAN_MASTER_H_

#include "stdlib.h"
#include "stdint.h"
#include "linked_list.h"
#include "CO.h"
#include "can_hal.h"

#define CAN_NODE_ID_ASSIGN_COBID						0x70
#define SLAVE_SERIAL_NUMBER_OBJECT_INDEX				0x2101

#define SDO_RX_BUFFER_SIZE                 (32UL)
typedef struct CO_SDO_SERVER_t CO_SDO_SERVER;

#define SDO_CS_INIT_READ                0
#define SDO_CS_SEGMENT_READ             1
#define SDO_CS_FINISH_READ              2

#define SDO_CS_INIT_WRITE               4
#define SDO_CS_SEGMENT_WRITE            5
#define SDO_CS_FINISH_WRITE             6
#define SDO_CS_ABORT                    7

typedef enum SDO_STATE_t{
        SDO_ST_IDLE=        0,
        SDO_ST_SENT=			1,
	SDO_ST_SUCCESS=	2,
	SDO_ST_FAIL=			3
}SDO_STATE;
struct CO_SDO_SERVER_t{
        uint32_t timeout;
        SDO_STATE state;
        uint16_t rx_index;
        uint16_t tx_index;
        uint32_t tx_address;
        uint32_t rx_address;
        uint32_t object_mux;
        uint8_t* rx_data_buff;
        uint8_t* tx_data_buff;
        uint8_t tx_msg_data[8];
        uint8_t rx_msg_data[8];
        uint32_t buff_offset;
        uint32_t object_data_len;
        uint8_t is_new_msg;
};

typedef struct CAN_master_t CAN_master;

typedef void (*CAN_Master_Slave_Select)(const CAN_master*,const uint32_t);

typedef enum CM_ASSIGN_STATE{
	CM_ASSIGN_ST_START,
	CM_ASSIGN_ST_SLAVE_SELECT,
	CM_ASSIGN_ST_SLAVE_SELECT_CONFIRM,
	CM_ASSIGN_ST_WAIT_CONFIRM,
	CM_ASSIGN_ST_AUTHORIZING,
	CM_ASSIGN_ST_DONE
} CM_ASSIGN_STATE;

struct CAN_master_t{
	uint8_t is_active;
	uint32_t slave_num;
	uint32_t slave_start_node_id;
	uint32_t node_id_scan_cobid;
	CM_ASSIGN_STATE	assign_state;
	uint32_t assign_timeout;
	CO_Slave**		slaves;
	CO_Slave* assigning_slave;
	CO_Slave* authorizing_slave;
	uint16_t 		time_stamp;
	CO_SDO_SERVER 	sdo_server;
	CAN_Hw* p_hw;
	CAN_Master_Slave_Select slave_select;
	CAN_Master_Slave_Select slave_deselect;
	void (*on_slave_assign_success)(const CAN_master* const p_cm,uint32_t slave_id);
	void (*on_slave_assign_fail)(const CAN_master* const p_cm,uint32_t slave_id);
};

void can_master_init(CAN_master* p_cm,CO_Slave** slaves,
		const uint32_t slave_num,CAN_Hw* p_hw);
void can_master_process(CAN_master* p_cm,const uint32_t timestamp);
void can_master_start_assign_next_slave(CAN_master* p_cm);
void can_master_update_id_assign_process(CAN_master* p_cm,const uint32_t timestamp);
void can_master_read_slave_sn(CAN_master* p_cm, uint8_t slave_id);
void cm_start_authorize_slave(CAN_master* p_cm,CO_Slave* slave);
void can_master_send_sync_request(CAN_master* p_cm,const uint32_t timestamp);

static inline void can_master_slave_select(const CAN_master* p_cm,
		const uint32_t id){
	p_cm->slave_select(p_cm,id);
}

static inline SDO_STATE sdo_server_get_state(const CO_SDO_SERVER* const p_svr){
        return p_svr->state;
}

static inline void sdo_server_set_state(CO_SDO_SERVER* p_svr,
		const SDO_STATE state){
        p_svr->state=state;
}

#endif /* SERVICE_CAN_MASTER_CAN_MASTER_H_ */
