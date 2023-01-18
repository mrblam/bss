/*
 * cabinet_app.h
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#ifndef CABINET_APP_CABINET_APP_H_
#define CABINET_APP_CABINET_APP_H_

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "app_config.h"
#include "can_master.h"
#include "cabinet_cell.h"
#include "ioe.h"
#include "bss_data.h"
#include "rs485_master.h"

typedef struct Cabinet_App_t Cabinet_App;

extern Cabinet_App selex_bss_app;

typedef enum CABINET_APP_STATE{
	CAB_APP_ST_SETUP,
	CAB_APP_ST_STANDBY,
	CAB_APP_ST_FAIL
} CABINET_APP_STATE;

typedef enum CMD_CODE{
	HMI_WRITE 	= 'W',
	HMI_READ 	= 'R'
} CMD_CODE;

typedef enum MAIN_OBJS{
	BSS_BP		= 'B',
	BSS_CABINET = 'C',
	BSS_STATION	= 'S'
} MAIN_OBJS;

typedef enum SUB_OBJS{
	ALL				= 'A',
	BSS_ID_ASSIGN	= 'I',
	BSS_AUTHORIZE	= 'U',
	DOOR			= 'D',
	FAN				= 'F',
	CHARGER			= 'C',
	LAMP			= 'L',
	STATE			= 'S',
	OP_STATE 		= 'O',
	SYNC_DATA		= 'Y',
	CAB_NUM			= 'N',
	TEMP			= 'T',
	S2_CAMEL_SN		= 'M',
	BSS_SN			= 'B'
} SUB_OBJS;

#define STATE_OK		'O'
#define STATE_FAIL		'F'

typedef struct CSV_t CSV;
struct CSV_t{
	CMD_CODE		cmd_code[AVAILABLE_HMI_MSG_NUM];
	uint8_t			id[AVAILABLE_HMI_MSG_NUM];
	MAIN_OBJS		main_obj[AVAILABLE_HMI_MSG_NUM];
	SUB_OBJS		sub_obj[AVAILABLE_HMI_MSG_NUM];
	uint8_t			obj_state[AVAILABLE_HMI_MSG_NUM];
	uint8_t			is_new_data;
	uint8_t			is_new_msg_to_send[AVAILABLE_HMI_MSG_NUM];
	uint8_t 		valid_msg_num;
};

struct Cabinet_App_t{
	CAN_master			base;
	CABINET_APP_STATE	state;
	CSV					hmi_csv;
	RS485_Master*		slave_com;
	BSS_Data			bss;
	uint8_t				rx_data[32];
	uint8_t				rx_index;
	uint8_t				is_new_msg;
	uint8_t*			start_msg_index;
	uint8_t 			is_hmi_req_sync;
	uint32_t			charge_timestamp;
};

extern uint32_t sys_timestamp;

void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca);
void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_active_charge(Cabinet_App* p_ca,uint8_t cab_id, const uint32_t timestamp);
void cab_app_deactive_charge(Cabinet_App* p_ca, uint8_t cab_id, const uint32_t timestamp);
void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint32_t timestamp);
void cab_app_check_buffer(Cabinet_App* p_ca);
void cab_app_send_msg_to_hmi(Cabinet_App* p_ca);
void cab_app_update_charge(Cabinet_App* p_ca, const uint32_t timestamp);
void cab_app_update_connected_cab_state(Cabinet_App* p_app);

static inline CABINET_APP_STATE cab_app_get_state(Cabinet_App* p_ca){
	return p_ca->state;
}

static inline void cab_app_set_state(Cabinet_App* p_ca, CABINET_APP_STATE state){
	p_ca->state = state;
}


#endif /* CABINET_APP_CABINET_APP_H_ */
