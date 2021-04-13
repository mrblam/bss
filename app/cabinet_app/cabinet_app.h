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
#include "app_config.h"
#include "cabinet_cell.h"
#include "ioe.h"
#include "mux.h"
#include "uart.h"
#include "can_master.h"
#include "linked_list.h"

typedef struct Cabinet_app_t Cabinet_app;

typedef enum CABIN_STATE{
	CABIN_ST_SETUP = 0,
	CABIN_ST_STANDBY = 1,
	CABIN_ST_ACTIVE = 2,
	CABIN_ST_MAINTAIN = 3,
	CABIN_ST_FAULT = 4,
} CABIN_STATE;

typedef enum TIMING_STATE{
	TIMING_ST_DEACTIVE = 0,
	TIMING_ST_ACTIVE = 1
} TIMING_STATE;

struct Cabinet_app_t{
	CAN_master		base;
	Cabinet_cell*	cabin[CABINET_CELL_NUM];
	IOE*			ioe_sol;
	IOE*			ioe_cfan;
	CABIN_STATE		state;
	TIMING_STATE	timing_state;
	uint8_t 		time_stamp;
	Cabinet_list*	empty_cab;
	Cabinet_list*	full_cab;
};

Cabinet_app* cab_app_construct(void);
void cab_app_init(Cabinet_app* p_ca);

void cab_app_set_state(Cabinet_app* p_ca, CABIN_STATE state);
CABIN_STATE cab_app_get_state(Cabinet_app* p_ca);

void cab_app_start_id_assign(Cabinet_app* p_ca);
Cabinet_cell* cab_app_select_empty_cabin(Cabinet_app* p_ca);
void cab_app_process_id_assign(Cabinet_app* p_ca);
void cab_app_update_cab_node_id(Cabinet_app* p_ca);
void cab_app_update_cabin_list(Cabinet_app* p_ca);

void cab_app_active_timing_state(Cabinet_app* p_ca, uint16_t time_ms);
void cab_app_deactive_timing_state(Cabinet_app* p_ca);

void cab_app_active_charge(Cabinet_app* p_ca,CABIN_ID cab_id);
void cab_app_deactive_charge(Cabinet_app* p_ca, CABIN_ID cab_id);
void cab_app_receive_bp(Cabinet_app* p_ca, CABIN_ID cab_id);
void cab_app_delivery_bp(Cabinet_app* p_ca, CABIN_ID cab_id);
void cab_app_check_bp_state(Cabinet_app* p_ca, CABIN_ID cab_id);

void cab_app_update_tilt_ss(Cabinet_app* p_ca);
void cab_app_check_hmi_msg(Cabinet_app* p_ca);
void cab_app_check_sim_msg(Cabinet_app* p_ca);
void cab_app_stream_data_hmi(Cabinet_app* p_ca);
void cab_app_stream_data_sim(Cabinet_app* p_ca);
void cab_app_send_warning_msg(Cabinet_app* p_ca);

#endif /* CABINET_APP_CABINET_APP_H_ */
