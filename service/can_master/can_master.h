/*
 * can_master.h
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */
/*
 * SDO : Serial number,on/off BP
 * PDO : SYNC data BP(Voltage,Current,)
 * CAN normal ; assign ID BP (5 + 1,2,3,4)
 * */
#ifndef SERVICE_CAN_MASTER_CAN_MASTER_H_
#define SERVICE_CAN_MASTER_CAN_MASTER_H_

#include "stdlib.h"
#include "stdint.h"
//#include "linked_list.h"
#include "CO.h"
#include "can_hal.h"

#define SDO_READ_OBJ_TIMEOUT_mS							2000
#define SDO_WRITE_OBJ_TIMEOUT_mS						2000

#define CAN_NODE_ID_ASSIGN_COBID						0x70
#define CAN_REQ_SYNC_BP_DATA_COBID						0x80
/*SDO Index and Sub-Index*/

#define SLAVE_SERIAL_NUMBER_OBJECT_INDEX				0x2101 //old libCAN
#define BMS_MAINSWITCH_INDEX							0x2109 //old libCAN

#define BMS_INDEX										0x2003
#define BMS_SERIAL_NUMBER_OBJECT_SUB_INDEX				0x00
#define BMS_MAINSWITCH_SUB_INDEX						0x01
#define SLAVE_ID_NUMBER_OBJECT_SUB_INDEX				0x02
#define BMS_VERSION_INDEX								0x100A
#define BMS_VERSION_SUBINDEX							0x00
#define BMS_VEHICLE_SN_INDEX							0x2004
#define BMS_MATTED_DEV_SUBINDEX							0x01

#define BMS_STATE_CHARGING								2
#define BMS_STATE_DISCHARGING							0

#define SLAVE_RPDO1										0x200
#define SDO_RX_BUFFER_SIZE                 				(32UL)
typedef struct CO_SDO_SERVER_t CO_SDO_SERVER;

#define SDO_CS_INIT_READ                0
#define SDO_CS_SEGMENT_READ             1
#define SDO_CS_FINISH_READ              2

#define SDO_CS_INIT_WRITE               4
#define SDO_CS_SEGMENT_WRITE            5
#define SDO_CS_FINISH_WRITE             6
#define SDO_CS_ABORT                    7

#define BP_VOL_CUR_TPDO_COBID                           CO_CAN_ID_TPDO_1
#define BP_CELLS_VOL_1_TO_4         		            CO_CAN_ID_TPDO_2
#define BP_CELLS_VOL_5_TO_8			                    CO_CAN_ID_TPDO_3
#define BP_CELLS_VOL_9_TO_12							CO_CAN_ID_RPDO_1
#define BP_CELLS_VOL_13_TO_16							CO_CAN_ID_RPDO_2
#define BP_TEMP_TPDO_COBID                              CO_CAN_ID_TPDO_4
//
//uint8_t serial_number_var[10];
//CO_Sub_Object serial_number_sobj =
//	{
//			.p_data = serial_number_var,	//<< Address variable receiving data
//			.attr	= ODA_SDO_RW,			//<< [skip] set ODA_SDO_RW
//			.len	= 10,					//<< Maximum data size that can be received
//			.p_ext	= NULL					//<< [option], set NULL if not used
//	};

typedef enum BMS_OBJ{
	BMS_MAINSWITCH	= 0,
	BMS_MATTING		= 1,
	BMS_MATED_DEV	= 2,
	BMS_NEW_FW_REQ	= 3
}BMS_OBJ;

typedef enum SDO_STATE_t{
	SDO_ST_IDLE 	= 0,
	SDO_ST_SENT		= 1,
	SDO_ST_SUCCESS	= 2,
	SDO_ST_FAIL		= 3
}SDO_STATE;

struct CO_SDO_SERVER_t{
	uint32_t 		timeout;
	SDO_STATE 		state;
	uint16_t 		rx_index;
	uint16_t 		tx_index;
	uint32_t 		tx_address;
	uint32_t 		rx_address;
	uint32_t 		object_mux;
	uint8_t* 		rx_data_buff;
	uint8_t* 		tx_data_buff;
	uint8_t 		tx_msg_data[8];
	uint8_t 		rx_msg_data[8];
	uint32_t 		buff_offset;
	uint32_t 		object_data_len;
	uint8_t 		is_new_msg;
	uint32_t 		node_id_processing;
	};

typedef struct CO_ReadPDO_t CO_ReadPDO;
struct CO_ReadPDO_t{
	uint32_t 		cob_id;
    uint8_t 		is_new_msg;
};

typedef struct CAN_master_t CAN_master;

typedef void (*CAN_Master_Slave_Select)(const CAN_master*,const uint32_t);

typedef enum CM_ASSIGN_STATE{
    CM_ASSIGN_ST_WAIT_REQUEST,
	CM_ASSIGN_ST_START,
	CM_ASSIGN_ST_SLAVE_SELECT,
	CM_ASSIGN_ST_SLAVE_SELECT_CONFIRM,
	CM_ASSIGN_ST_WAIT_CONFIRM,
	CM_ASSIGN_ST_AUTHORIZING,
	CM_ASSIGN_ST_DONE,
	CM_ASSIGN_ST_FAIL
} CM_ASSIGN_STATE;

typedef enum SN_ASSIGN_STATE{
	BMS_MATED_DEV_IDLE,
	BMS_MATED_DEV_CHECK_WRITE_SN_STATE,
	BMS_MATED_DEV_WRITE_BSS_SN,
	BMS_MATED_DEV_CHECK_READ_BSS_SN,
	BMS_MATED_DEV_DONE
} SN_ASSIGN_STATE;

typedef enum SDO_SERVICE{
	SDO_SERVICE_IDLE,
	SDO_SERVICE_ACTIVE_CHARGER,
	SDO_SERVICE_DEACTIVE_CHARGER,
	SDO_SERVICE_READ_SN_BP,
	SDO_SERVICE_WRITE_SN_XE,
	SDO_SERVICE_READ_SN_XE,
	SDO_SERVICE_READ_BP_SW_VERSION,
	SDO_SERVICE_BOOT_BMS,
	SDO_SERVICE_REQ_BOOT_BMS
}SDO_SERVICE;

struct CAN_master_t{
	CO 							CO_base; // new
	uint8_t 					is_active;
	uint32_t 					slave_num;
	uint32_t 					slave_start_node_id;
	uint32_t 					node_id_scan_cobid;
	CM_ASSIGN_STATE				assign_state;
	SN_ASSIGN_STATE				sn_assign_state;
	uint32_t 					assign_timeout;
	CO_Slave**					slaves;
	CO_Slave* 					assigning_slave;
	CO_Slave* 					authorizing_slave;
	uint16_t 					time_stamp;
	CO_SDO_SERVER 				sdo_server;
	uint32_t 					pdo_sync_timestamp;
	CAN_Hw*						p_hw;
	CAN_Master_Slave_Select 	slave_select;
	CAN_Master_Slave_Select 	slave_deselect;
	CO_Sub_Object 				serial_number_sobj;
	CO_Sub_Object				data_write_bms_od;
	uint8_t						bms_mainswitch_state;
	uint8_t						bms_matting_state;
	uint8_t						bms_new_fw_req;
	uint8_t						hmi_xe_sn[32];
	uint8_t						slave_id;
	uint8_t						camel_sn[32];
	SDO_SERVICE					sdo_service;
	int							write;
	int							write_success;
	int							write_abort;
	int							read;
	int							read_success;
	int							read_abort_sdo_success;
	int							read_abort;
	bool						sdo_service_xe_sn_done;
	void (*on_slave_assign_success)(const CAN_master* const p_cm,uint32_t slave_id);
	void (*on_slave_assign_fail)(CAN_master* p_cm,uint32_t slave_id);
	void (*reassign_attemp)(CAN_master* p_cm);
	void (*rpdo_process)(const CAN_master* const p_cm);
	void 				(*read_serial_number_bp)(void);
	bool 				(*sdo_write_object)(void);
	uint32_t					sdo_timeout;
	bool						sdo_finish;
	uint32_t					authorizing_timeout;
};

extern uint8_t reassign_attemp_cnt;

void can_master_init(CAN_master* p_cm,CO_Slave** slaves, const uint32_t slave_num,CAN_Hw* p_hw);
void can_master_process(CAN_master* p_cm,const uint32_t timestamp);
void can_master_start_assign_next_slave(CAN_master* p_cm,const uint32_t timestamp);
void can_master_update_id_assign_process(CAN_master* p_cm,const uint32_t timestamp);
void can_master_update_sn_assign_process(CAN_master* p_cm);
void can_master_read_slave_sn(CAN_master* p_cm, uint8_t cab_id, uint32_t timestamp);
void can_master_write_bms_object(CAN_master* p_cm, uint8_t cab_id, BMS_OBJ bms_obj);
void cm_start_authorize_slave(CAN_master* p_cm,CO_Slave* slave, uint32_t timestamp);
void can_master_send_sync_request(CAN_master* p_cm,const uint32_t timestamp);
void co_sdo_read_object(CAN_master* p_cm,const uint32_t mux,const uint32_t node_id,uint8_t* rx_buff,const uint32_t timeout);
void co_sdo_write_object(CAN_master* p_cm,const uint32_t mux,const uint32_t node_id,
		uint8_t* tx_buff,const uint32_t len,const uint32_t timeout);
void can_master_start_assign_slave(CAN_master* p_cm, CO_Slave *slave, const uint32_t timestamp);
void can_master_disable_pdo(CAN_master* p_cm);
void can_set_read_sn_func_pointer(CAN_master* p_cm,void (*read_serial_number_bp)(void));
void can_set_sdo_write_obj_func_pointer(CAN_master* p_cm,void (*sdo_write_object)(void));
void can_master_read_bp_version_software(CAN_master* p_cm, uint8_t cab_id);
static inline void can_master_slave_select(const CAN_master* p_cm, const uint32_t id){
	p_cm->slave_select(p_cm,id);
}

static inline void can_master_slave_deselect(const CAN_master* p_cm, const uint32_t id){
	p_cm->slave_deselect(p_cm,id);
}

static inline SDO_STATE sdo_server_get_state(const CO_SDO_SERVER* const p_svr){
	return p_svr->state;
}

static inline void sdo_server_set_state(CO_SDO_SERVER* p_svr, const SDO_STATE state){
	p_svr->state=state;
}

#endif /* SERVICE_CAN_MASTER_CAN_MASTER_H_ */
