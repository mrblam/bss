/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"

void cab_app_init(Cabinet_app* p_ca){
	can_master_init((CAN_master*) p_ca);
	p_ca->state = CABIN_ST_SETUP;
	p_ca->empty_cab = cab_list_init();
	p_ca->full_cab = cab_list_init();

	p_ca->state = CABIN_ST_ACTIVE;
	for(uint8_t cab_id = CAB1 - 1; cab_id < CAB15; cab_id++){
		p_ca->cabin[cab_id] = cab_cell_construct();
		p_ca->cabin[cab_id]->cab_id = cab_id + 1;
		p_ca->cabin[cab_id]->node_id = DEFAULT_BP_ID;
		cab_list_insert_to_tail(p_ca->empty_cab, p_ca->cabin[cab_id]);
	}

	p_ca->ioe_cfan = ioe_construct();
	p_ca->ioe_sol = ioe_construct();
}

void cab_app_set_state(Cabinet_app* p_ca, CABIN_STATE state){

}

CABIN_STATE cab_app_get_state(Cabinet_app* p_ca){
	return 0;
}

/* Enable timeout process */
void cab_app_active_timing_state(Cabinet_app* p_ca, uint16_t time_ms){
	p_ca->timing_state = TIMING_ST_ACTIVE;
	p_ca->time_stamp = time_ms;
}

/* Disable timeout process */
void cab_app_deactive_timing_state(Cabinet_app* p_ca){
	p_ca->timing_state = TIMING_ST_DEACTIVE;
	p_ca->time_stamp = 0;
}

void cab_app_active_charge(Cabinet_app* p_ca,CABIN_ID cab_id){

}

void cab_app_deactive_charge(Cabinet_app* p_ca, CABIN_ID cab_id){

}

void cab_app_receive_bp(Cabinet_app* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(p_ca->cabin[cab_id]);
	cab_cell_check_door_state(p_ca->cabin[cab_id]);
}

void cab_app_delivery_bp(Cabinet_app* p_ca, CABIN_ID cab_id){

}

void cab_app_check_bp_state(Cabinet_app* p_ca, CABIN_ID cab_id){

}

void cab_app_update_tilt_ss(Cabinet_app* p_ca){

}

void cab_app_sync_bss_data_hmi(Cabinet_app* p_ca){

}

void cab_app_sync_cab_data_hmi(Cabinet_app* p_ca){

}

void cab_app_sync_bp_data_hmi(Cabinet_app* p__ca){

}

void cab_app_stream_data_hmi(Cabinet_app* p_ca){

}

void cab_app_stream_data_sim(Cabinet_app* p_ca){

}

void cab_app_send_warning_msg(Cabinet_app* p_ca){

}
