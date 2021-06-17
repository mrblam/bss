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
#include "linked_list.h"
#include "bss_data.h"
#include "rs485_master.h"

typedef struct Cabinet_App_t Cabinet_App;

typedef enum CABINET_APP_STATE{
	CAB_APP_ST_SETUP,
	CAB_APP_ST_STANDBY,
	CAB_APP_ST_FAIL
} CABINET_APP_STATE;

typedef enum MAIN_OBJS{
	BSS_BP		= 'B',
	BSS_CABINET = 'C',
	BSS_STATION	= 'S'
} MAIN_OBJS;

typedef enum SUB_OBJS{
	BSS_ID_ASSIGN	= 'I',
	BSS_AUTHORIZE	= 'A',
	DOOR			= 'D',
	FAN				= 'F',
	CHARGER			= 'C',
	LAMP			= 'L',
	STATE			= 'S',
	OP_STATE 		= 'O',
	SYNC_DATA		= 'Y',
	CAB_NUM			= 'N'
} SUB_OBJS;

#define AUTH_OK		'O'
#define AUTH_FAIL	'F'

typedef struct CSV_t CSV;
struct CSV_t{
	uint8_t 		valid_msg_num;
	uint8_t			id[AVAILABLE_HMI_MSG_NUM];
	MAIN_OBJS		main_obj[AVAILABLE_HMI_MSG_NUM];
	SUB_OBJS		sub_obj[AVAILABLE_HMI_MSG_NUM];
	uint8_t			obj_state[AVAILABLE_HMI_MSG_NUM];
	uint8_t			is_new_data;
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
};

void cab_app_receive_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca);
void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_active_charge(Cabinet_App* p_ca,uint8_t cab_id);
void cab_app_deactive_charge(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_parse_hmi_msg(Cabinet_App* p_ca);
void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint32_t timestamp);
void cab_app_check_buffer(Cabinet_App* p_ca);

static inline CABINET_APP_STATE cab_app_get_state(Cabinet_App* p_ca){
	return p_ca->state;
}

static inline void cab_app_set_state(Cabinet_App* p_ca, CABINET_APP_STATE state){
	p_ca->state = state;
}


#endif /* CABINET_APP_CABINET_APP_H_ */
