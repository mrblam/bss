/*
 * cabinet_app.h
 *
 *  Created on: Mar 25, 2021
 *      Author: KhanhDinh
 */

#ifndef APP_CABINET_APP_CABINET_APP_H_
#define APP_CABINET_APP_CABINET_APP_H_

#include "can_master.h"
#include "linked_list.h"

#define CELL_NUM	15

typedef enum CELL_ID{
	CELL1 = 1,
	CELL2 = 2,
	CELL3 = 3,
	CELL4 = 4,
	CELL5 = 5,
	CELL6 = 6,
	CELL7 = 7,
	CELL8 = 8,
	CELL9 = 9,
	CELL10 = 10,
	CELL11 = 11,
	CELL12 = 12,
	CELL13 = 13,
	CELL14 = 14,
	CELL15 = 15
} CELL_ID;

typedef enum CABIN_STATE{
	CABIN_ST_SETUP = 0,
	CABIN_ST_MAINTAIN = 1,
	CABIN_ST_OPERATIONAL = 2,
	CABIN_ST_FAULT = 3
} CABIN_STATE;

typedef enum TIMING_STATE{
	TIMIMG_ST_DEACTIVE = 0,
	TIMING_ST_ACTIVE = 1
} TIMING_STATE;

typedef enum TILT_SS_STATE{
	TILT_ST_DEACTIVE = 0,
	TILT_ST_ACTIVE = 1
} TILT_SS_STATE;

typedef enum WARNING_STATE{
	NO_WARNING = 0,
	WARNING_LOST_POWER = 1,
	WARNING_MOVEMENT = 2,
	WARNING_CHARGER_ERROR = 3,
	WARNING_LOST_BP = 4
}WARNING_STATE;

typedef enum ASSIGN_STATE{
	ASSIGN_FAIL = 0,
	ASSIGN_SUCCESS = 1
} ASSIGN_STATE;

typedef enum CELL_STATE{
	CELL_OK = 0,
	CELL_ERROR = 1
} CELL_STATE;

struct Cabinet_App_t {
	CAN_Master 		base;
	uint8_t 		cell_id;
	CABIN_STATE 	state;
	TIMING_STATE 	timing_state;
	TILT_SS_STATE 	tilt_ss_state;
	WARNING_STATE	warning_state;
	uint16_t 		timing_ms;
	uint8_t 		cell_num;
	LIST*			empty_cell;
	LIST*			busy_cell;
} ;
typedef struct Cabinet_App_t Cabinet_App;

Cabinet_App* ca_construct(void);
void ca_init(Cabinet_App* p_ca);
void ca_update_state(Cabinet_App* p_ca, CABIN_STATE state);
CABIN_STATE ca_get_state(Cabinet_App* p_ca);

void ca_accept_assign_id(Cabinet_App* p_ca);
CELL_STATE ca_select_cell(Cabinet_App* p_ca, LIST* p_list);
void ca_deselect_cell(Cabinet_App* p_ca, uint8_t cell_id);
void ca_process_assign_id(Cabinet_App* p_ca);
ASSIGN_STATE ca_start_assign_id(Cabinet_App* p_ca, uint16_t timestamp);
void ca_active_timing_state(Cabinet_App* p_ca, uint16_t timestamp_ms);
void ca_deactive_timing_state(Cabinet_App* p_ca);
void ca_process_timing(Cabinet_App* p_ca);

void ca_update_cell_state(Cabinet_App* p_ca, CELL_ID cell_id);
TILT_SS_STATE ca_update_tilt_state(Cabinet_App* p_ca);
void ca_update_cell_temp(Cabinet_App* p_ca);
void ca_active_cell_door(Cabinet_App* p_ca, CELL_ID cell_id);
void ca_deactive_cell_door(Cabinet_App* p_ca, CELL_ID cell_id);
void ca_active_cell_charge(Cabinet_App* p_ca, CELL_ID cell_id);
void ca_deactive_cell_charge(Cabinet_App* p_ca, CELL_ID cell_id);
void ca_active_cell_fan(Cabinet_App* p_ca, CELL_ID cell_id);
void ca_deactive_cell_fan(Cabinet_App* p_ca, CELL_ID cell_id);

void ca_stream_data_hmi(Cabinet_App* p_ca);
void ca_check_hmi_msg(Cabinet_App* p_ca);
void ca_stream_data_sim(Cabinet_App* p_ca);
void ca_check_sim_msg(Cabinet_App* p_ca);
void ca_warning_server(Cabinet_App* p_ca, WARNING_STATE warning_state);

#endif /* APP_CABINET_APP_CABINET_APP_H_ */
