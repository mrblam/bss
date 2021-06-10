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
	LAMP			= 'L',
	STATE			= 'S'
} SUB_OBJS;

typedef enum OBJ_STATE{
	SW_DEACTIVE	= '0',
	SW_ACTIVE	= '1',
	SW_FAIL		= '2',
	AUTH_START	= 'S',
	AUTH_OK		= 'O',
	AUTH_FAIL	= 'F'
} OBJ_STATE;

typedef struct CSV_t CSV;
struct CSV_t{
	uint8_t id;
	MAIN_OBJS		main_obj;
	SUB_OBJS		sub_obj;
	OBJ_STATE		obj_state;
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
};

void cab_app_receive_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca);
void cab_app_sync_bp_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);
void capp_on_cabinet_door_close(Cabinet_App* p_app,Cabinet* p_cab);
void cab_app_active_charge(Cabinet_App* p_ca,uint8_t cab_id);
void cab_app_deactive_charge(Cabinet_App* p_ca, uint8_t cab_id);
void cab_app_parse_hmi_msg(Cabinet_App* p_ca);
void cab_app_process_hmi_command(Cabinet_App* p_ca, const uint32_t timestamp);

static inline CABINET_APP_STATE cab_app_get_state(Cabinet_App* p_ca){
	return p_ca->state;
}

static inline void cab_app_set_state(Cabinet_App* p_ca, CABINET_APP_STATE state){
	p_ca->state = state;
}


#endif /* CABINET_APP_CABINET_APP_H_ */
