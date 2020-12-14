#ifndef CO_H_
#define CO_H_

#include "../../../libs/selex-libc/canopen_clib/CAN_Msg.h"
#include "../../../libs/selex-libc/canopen_clib/CO_RPDO.h"
#include "../../../libs/selex-libc/canopen_clib/CO_SDO.h"
#include "../../../libs/selex-libc/canopen_clib/CO_TPDO.h"

#define LIB_USED_RAMFUNC				0
#define LIB_USED_OPTIMIZE_SPEED_ON			0
#define LIB_FORCE_INLINE    inline __attribute__((always_inline))

#if LIB_USED_OPTIMIZE_SPEED_ON
#define OPTIMIZE_SPEED_ON   __attribute__((optimize("O3")))
#else
#define LIB_OPTIMIZE_SPEED_ON
#endif

#if LIB_USED_RAMFUNC
#define LIB_RAM_FUNC  __attribute__((section(".data,\"aw\",%progbits @")))
#else
#define LIB_RAM_FUNC
#endif

#define LIB_OPTIMIZE_ATTR  LIB_FORCE_INLINE LIB_RAM_FUNC LIB_OPTIMIZE_SPEED_ON

typedef struct CO_t CO;
typedef void (*CO_Sends)(CO* p_dev,const CAN_Msg* p_msg);
typedef void (*CO_Error_Handle)(CO* p_dev);
extern uint8_t SOM[2];

#define CO_TX_BUFFER_SIZE		128
#define CO_RX_BUFFER_SIZE		128

#define SDO_CLIENT_TO_SERVER            600UL
#define SDO_SERVER_TO_CLIENT            580UL
#define NODE_ID                         3U

#define FW_INDEX			2000UL
#define FW_SUB_INDEX			2U

#define RX_MSG_BUFFER_SIZE			16
#define RPDO_NUMBER                      4
#define TPDO_NUMBER                      4

#define SDO_NUMBER                      1

typedef enum CO_STATE_t CO_STATE;

/**
 * Default CANopen identifiers.
 *
 * Default CANopen identifiers for CANopen communication objects. Same as
 * 11-bit addresses of CAN messages. These are default identifiers and
 * can be changed in CANopen. Especially PDO identifiers are confgured
 * in PDO linking phase of the CANopen network configuration.
 */
typedef enum{
     CO_CAN_ID_NMT_SERVICE       = 0x000,   /**< 0x000, Network management */
     CO_CAN_ID_SYNC              = 0x080,   /**< 0x080, Synchronous message */
     CO_CAN_ID_EMERGENCY         = 0x080,   /**< 0x080, Emergency messages (+nodeID) */
     CO_CAN_ID_TIME_STAMP        = 0x100,   /**< 0x100, Time stamp message */
     CO_CAN_ID_TPDO_1            = 0x180,   /**< 0x180, Default TPDO1 (+nodeID) */
     CO_CAN_ID_RPDO_1            = 0x200,   /**< 0x200, Default RPDO1 (+nodeID) */
     CO_CAN_ID_TPDO_2            = 0x280,   /**< 0x280, Default TPDO2 (+nodeID) */
     CO_CAN_ID_RPDO_2            = 0x300,   /**< 0x300, Default RPDO2 (+nodeID) */
     CO_CAN_ID_TPDO_3            = 0x380,   /**< 0x380, Default TPDO3 (+nodeID) */
     CO_CAN_ID_RPDO_3            = 0x400,   /**< 0x400, Default RPDO3 (+nodeID) */
     CO_CAN_ID_TPDO_4            = 0x480,   /**< 0x480, Default TPDO4 (+nodeID) */
     CO_CAN_ID_RPDO_4            = 0x500,   /**< 0x500, Default RPDO5 (+nodeID) */
     CO_CAN_ID_TSDO              = 0x580,   /**< 0x580, SDO response from server (+nodeID) */
     CO_CAN_ID_RSDO              = 0x600,   /**< 0x600, SDO request from client (+nodeID) */
     CO_CAN_ID_HEARTBEAT         = 0x700    /**< 0x700, Heartbeat message */
}CO_Default_CAN_ID_t;

#define CO_ERR_REG_GENERIC_BIT			(1<<0)
#define CO_ERR_REG_CURRENT_BIT			(1<<1)
#define CO_ERR_REG_VOLTAGE_BIT			(1<<2)
#define CO_ERR_REG_TEMPERATURE_BIT		(1<<3)
#define CO_ERR_REG_COMMUNICATION_BIT		(1<<4)
#define CO_ERR_REG_DEVICE_PROFILE_BIT		(1<<5)
#define CO_ERR_REG_SELEX_SPECIFIC_BIT		(1<<7)

#define ERR_HISTORY_SIZE			8

typedef struct CO_Interface_t CO_Interface;
struct CO_Interface_t{
        void (*co_start)(CO* p_dev);        
};

enum CO_STATE_t{

	CO_ST_NMT_INIT		=0,
	CO_ST_PRE_OPERATIONAL   =1,
	CO_ST_READY		=2,
	CO_ST_OPERATIONAL	=3,
        CO_ST_STOPPED           =4
};

typedef struct CO_Identity_t{
         uint32_t vendor_id;
         uint32_t product_code;
         uint32_t revisionNumber;
         uint32_t serial_number;
}CO_Identity;

typedef struct CO_Error_List_t{
	uint8_t error_no;
	uint32_t errors[ERR_HISTORY_SIZE];
}CO_Error_List;

typedef struct CO_Program_Data_t{
	uint8_t program_no;
	void* program_addr[8];
}CO_Program_Data;

struct CO_t{
	uint32_t ctrl_command;
	uint32_t device_type;/* VAR Device type */
	uint8_t error_reg; /* VAR Error register */
	CO_Error_List predefined_error_fields;
	uint32_t status;/* VAR Manufacturer status register */
	uint32_t error_code;
        uint32_t warning_code;
	uint8_t op_mode;
	uint32_t comm_cycle;/*Communication cycle period */
	uint32_t sync_window_len;/*Synchronous window length */
        char device_name[30];
	uint32_t hardware_version;
	uint32_t software_version;
	uint32_t guard_time;
	uint32_t timestamp;
	uint32_t high_resolution_timestamp;
	uint32_t consumer_heartbeat_time;
	uint32_t producer_heartbeat_time;
	uint32_t emergency_cob_id;
	CO_Identity identity;
        CO_OD* od;
        CAN_Msg rx_msgs[RX_MSG_BUFFER_SIZE];
	uint8_t node_id;
        CO_RPDO rpdos[RPDO_NUMBER];
        CO_TPDO tpdos[TPDO_NUMBER];
        CO_SDO sdos[SDO_NUMBER];
        CO_Sends sends;
        CO_Error_Handle error_handle;
};

void co_send_boost_msg(CO* p_co);

static LIB_OPTIMIZE_ATTR uint32_t co_get_last_error(const CO* p_dev){
	return p_dev->error_code;
}

static LIB_OPTIMIZE_ATTR void co_set_error(CO* p_dev,const uint32_t code){

	uint8_t error_index=0;
	/*
	 * shift all error records to right
	 * so the error record at index 0 always is newest error
	 */
	for(error_index=0;error_index<p_dev->predefined_error_fields.error_no-1;
			error_index++){

		p_dev->predefined_error_fields.errors[error_index+1]=
				p_dev->predefined_error_fields.errors[error_index];
	}

	p_dev->predefined_error_fields.error_no+=1;
	if(p_dev->predefined_error_fields.error_no>ERR_HISTORY_SIZE){
		p_dev->predefined_error_fields.error_no=ERR_HISTORY_SIZE;
	}

	p_dev->predefined_error_fields.errors[0]=code;
	p_dev->error_code=code;
}

CO* CO_create(const uint8_t node_id);
void CO_set_od(CO* p_co,CO_OD* p_od);
void CO_process(CO* p_co);
void CO_process_tpdo(CO* p_co);
void CO_process_rpdo(CO* p_co);
void CO_process_sdo(CO* p_co);
void CO_set_send_handle(CO* p_dev,CO_Sends sends);
void co_receive_msg(CO* p_dev,CAN_Msg* p_msg);

static inline void co_sends(CO* p_co,const CAN_Msg* p_msg){

        p_co->sends(p_co,p_msg);       
}

static inline void co_set_error_handle(CO* p_dev,CO_Error_Handle handle){

	p_dev->error_handle=handle;
}

static LIB_OPTIMIZE_ATTR void co_handle_error(CO* p_dev){
	p_dev->error_handle(p_dev);
}

#endif
