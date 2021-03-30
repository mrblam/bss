/*
 * cabinet_app.c
 *
 *  Created on: Mar 25, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"

Cabinet_App* ca_construct(void){
	Cabinet_App* p_ca = (Cabinet_App*) malloc(sizeof(Cabinet_App));
	return p_ca;
}

void ca_init(Cabinet_App* p_ca){
	cm_init((CAN_Master*)p_ca);

	p_ca->state = CABIN_ST_SETUP;
	p_ca->tilt_ss_state = TILT_ST_DEACTIVE;
	p_ca->warning_state = NO_WARNING;
	p_ca->timing_state = TIMIMG_ST_DEACTIVE;
	p_ca->timing_ms = 0;
	p_ca->cell_num = CELL_NUM;
	p_ca->empty_cell = list_init();
	for(uint8_t data = CELL1; data <= CELL15; data++){
		list_insert_to_tail(p_ca->empty_cell,data);
	}
	p_ca->busy_cell = list_init();

}

void ca_accept_assign_id(Cabinet_App* p_ca){
	cm_respone_assign_request((CAN_Master*) p_ca);
}

void ca_active_timing_state(Cabinet_App* p_ca, uint16_t timestamp_ms){
	p_ca->timing_ms = timestamp_ms;
	p_ca->timing_state = TIMING_ST_ACTIVE;
}

void ca_deselect_cell(Cabinet_App* p_ca, uint8_t cell_id){
	list_remove_node(p_ca->empty_cell, cell_id);
}

void ca_deactive_timing_state(Cabinet_App* p_ca){

}

CELL_STATE ca_select_cell(Cabinet_App* p_ca, LIST* p_list){
	p_ca->cell_id = list_walk_down(p_list);
	if(p_ca->cell_id == END_OF_LIST) return CELL_ERROR;
	else return CELL_OK;
}

ASSIGN_STATE ca_start_assign_id(Cabinet_App* p_ca, uint16_t timestamp_ms){
	ca_active_timing_state(p_ca, timestamp_ms);
	cm_send_assign_msg((CAN_Master*) p_ca);
	while((p_ca->timing_ms > 0) && (p_ca->timing_state == TIMING_ST_ACTIVE)){
		if(cm_wait_slave_confirm((CAN_Master*) p_ca) == SLAVE_OK){
			ca_deactive_timing_state(p_ca);
			return ASSIGN_SUCCESS;
		}
	}
	ca_deactive_timing_state(p_ca);
	return ASSIGN_FAIL;
}

void ca_process_assign_id(Cabinet_App* p_ca){
	while(cm_select_slave((CAN_Master*) p_ca, p_ca->base.empty_slave) != SLAVE_ERROR){
		while(ca_select_cell(p_ca, p_ca->empty_cell) != CELL_ERROR){
			if(ca_start_assign_id(p_ca, 5) != ASSIGN_FAIL){
				ca_deselect_cell(p_ca, p_ca->cell_id);
				cm_deselect_slave((CAN_Master*) p_ca, p_ca->base.slave_id);
				break;
			}
		}
		if(p_ca->empty_cell->p_temp == NULL) return;
	}
}


