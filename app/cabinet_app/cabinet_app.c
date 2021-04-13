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
	p_ca->cabin[0]->cell_fan->sw_on(p_ca->cabin[0]->cell_fan) = ioe_set((Switch*)p_ca->ioe_cfan);

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

/* Set all Node_ID Pin of Cabinet in Empty_cabin_list to High */
void cab_app_start_id_assign(Cabinet_app* p_ca){

	Cabinet_cell* p_temp = cab_list_walk_down(p_ca->empty_cab);
	while(p_temp != NULL){
		can_master_active_node_id_pin((CAN_master*)p_ca, p_temp->cab_id);
		p_temp = cab_list_walk_down(p_ca->empty_cab);
	}
	free(p_temp);
}

/* Select next Cabinet in Empty_cabin_list */
Cabinet_cell* cab_app_select_empty_cabin(Cabinet_app* p_ca){
	Cabinet_cell* p_temp = cab_list_walk_down(p_ca->empty_cab);
	if(p_temp == NULL) return p_temp;
	can_master_deactive_node_id_pin((CAN_master*)p_ca, p_temp->cab_id);
	return p_temp;
}

/* Implementing Node_ID assign for Cabinets in Empty_cabin_list */
void cab_app_process_id_assign(Cabinet_app* p_ca){
	can_master_response_id_assign_req((CAN_master*)p_ca);
	uint8_t temp_slave = can_master_select_empty_slave((CAN_master*)p_ca);
	Cabinet_cell* temp_cabin = cab_app_select_empty_cabin(p_ca);

	while(temp_slave != END_OF_LIST){
		while((temp_cabin != NULL) && (p_ca->base.assign_state != ASSIGN_ST_OK)){
			can_master_send_id_msg((CAN_master*)p_ca);
			cab_app_active_timing_state(p_ca, 1);
			while(p_ca->time_stamp > 0){
				p_ca->base.assign_state = can_master_wait_slave_confirm((CAN_master*)p_ca);
				if(p_ca->base.assign_state == ASSIGN_ST_OK){
					cab_app_update_cab_node_id(p_ca);
					cab_app_update_cabin_list(p_ca);
					can_master_update_slave_list((CAN_master*)p_ca);
					cab_app_deactive_timing_state(p_ca);
					break;
				}
					p_ca->time_stamp--;
			}
			temp_cabin = cab_app_select_empty_cabin(p_ca);
		}
		temp_slave = can_master_select_empty_slave((CAN_master*)p_ca);
	}
}

/* Update new node_id of cabinet */
void cab_app_update_cab_node_id(Cabinet_app* p_ca){
	p_ca->empty_cab->p_temp->data->node_id = p_ca->base.empty_slave_list->p_temp->data;
}

/* Update Cabinet_list after a change */
void cab_app_update_cabin_list(Cabinet_app* p_ca){
	cab_list_remove_node(p_ca->empty_cab, p_ca->empty_cab->p_temp->data);
	cab_list_insert_to_tail(p_ca->full_cab, p_ca->empty_cab->p_temp->data);
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

void cab_app_check_hmi_msg(Cabinet_app* p_ca){

}

void cab_app_check_sim_msg(Cabinet_app* p_ca){

}

void cab_app_stream_data_hmi(Cabinet_app* p_ca){

}

void cab_app_stream_data_sim(Cabinet_app* p_ca){

}

void cab_app_send_warning_msg(Cabinet_app* p_ca){

}
