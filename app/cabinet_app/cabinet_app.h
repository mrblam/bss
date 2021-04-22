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
#include "cabinet_cell.h"
#include "ioe.h"
#include "mux.h"
#include "can_master.h"
#include "linked_list.h"
#include "bss_data.h"
#include "board.h"

typedef struct Cabinet_App_t Cabinet_App;

typedef enum CABIN_STATE{
	CABIN_ST_SETUP = 0,
	CABIN_ST_STANDBY = 1,
	CABIN_ST_ASSIGN_ID = 2,
	CABIN_ST_ACTIVE = 3,
	CABIN_ST_MAINTAIN = 4,
	CABIN_ST_FAULT = 5,
} CABIN_STATE;

struct Cabinet_App_t{
	CAN_master		base;
	//Cabinet_Cell*	cabin[CABINET_CELL_NUM];
	IOE*			ioe_sol;
	IOE*			ioe_cfan;
	CABIN_STATE		state;
	TIMING_STATE	timing_state;
	uint8_t 		time_stamp;
	//Cabinet_List*	empty_cabs;
	//Cabinet_List*	full_cabs;
	BSS_Data		bss;
};

Cabinet_App* cab_app_construct(void);
void cab_app_init(Cabinet_App* p_ca);

void cab_app_set_state(Cabinet_App* p_ca, CABIN_STATE state);
CABIN_STATE cab_app_get_state(Cabinet_App* p_ca);

void cab_app_start_id_assign(Cabinet_App* p_ca);
Cabinet_Cell* cab_app_select_empty_cabin(Cabinet_App* p_ca);
void cab_app_process_id_assign(Cabinet_App* p_ca);
void cab_app_update_cab_node_id(Cabinet_App* p_ca);
void cab_app_update_cabin_list(Cabinet_App* p_ca);

void cab_app_active_timing_state(Cabinet_App* p_ca, uint16_t time_ms);
void cab_app_deactive_timing_state(Cabinet_App* p_ca);

void cab_app_active_charge(Cabinet_App* p_ca,CABIN_ID cab_id);
void cab_app_deactive_charge(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_receive_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id);
void cab_app_check_bp_state(Cabinet_App* p_ca, CABIN_ID cab_id);

void cab_app_update_tilt_ss(Cabinet_App* p_ca);
void cab_app_check_hmi_msg(Cabinet_App* p_ca);
void cab_app_check_sim_msg(Cabinet_App* p_ca);
void cab_app_stream_data_hmi(Cabinet_App* p_ca);
void cab_app_stream_data_sim(Cabinet_App* p_ca);
void cab_app_send_warning_msg(Cabinet_App* p_ca);

void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca);
void cab_app_sync_bp_data_hmi(__attribute__((unused)) Cabinet_App* p_ca, BP* p_bp);
void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id);

void cab_app_decode_cmd_hmi(Cabinet_App* p_ca, char* buff);
void cab_app_process_cab_cmd_hmi(Cabinet_App* p_ca, char* token);
void cab_app_start_id_assign(Cabinet_App* p_ca);

#endif /* CABINET_APP_CABINET_APP_H_ */
