/*
 * cabinet_app.c
 *
 *  Created on: Apr 5, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"
#include "uart_hw_hal.h"

static void can_master_slave_select_impl(const CAN_master* p_cm,const uint32_t id);
void node_id_pin_active(uint8_t cab_id);
void node_id_pin_deactive(uint8_t cab_id);

static Cabinet bss_cabinets[CABINET_CELL_NUM];
static CO_Slave* bp_slaves[CABINET_CELL_NUM];

void cab_app_init(Cabinet_App* p_ca){
	p_ca->state = CABIN_ST_SETUP;
	p_ca->bss.cabs=&bss_cabinets[0];
	for(int i=0;i<CABINET_CELL_NUM;i++){
	        bss_cabinets[i].cab_id=i;
	        bss_cabinets[i].bp=(BP*)malloc(sizeof(BP));
	        while(bss_cabinets[i].bp==NULL);
	        bss_cabinets[i].bp->can_node_id=BP_DEFAULT_CAN_NODE_ID;
	        bp_slaves[i]=(CO_Slave*)(bss_cabinets[i].bp);
	        bp_slaves[i]->con_state=CO_SLAVE_CON_ST_DISCONNECT;
	        bp_slaves[i]->node_id=CABINET_START_NODE_ID+i;
	        bp_slaves[i]->sdo_server_address=0x580+bp_slaves[i]->node_id;
	}

	p_ca->base.slave_start_node_id=CABINET_START_NODE_ID;
	can_master_init((CAN_master*)p_ca,&(bp_slaves[0]),CABINET_CELL_NUM,&can_port);
	p_ca->base.node_id_scan_cobid=TSDO_ID;
	p_ca->base.slave_select=can_master_slave_select_impl;

	p_ca->ioe_cfan = ioe_construct();
	p_ca->ioe_sol = ioe_construct();
}

void ca_update_cabinet_state(Cabinet_App* p_ca){
	bss_update_cabinets_state(&p_ca->bss);
}

static void can_master_slave_select_impl(const CAN_master* p_cm,const uint32_t id){
	(void)p_cm;
	node_id_pin_active(id);
}


void cab_app_active_charge(Cabinet_App* p_ca,CABIN_ID cab_id){
	cab_cell_active_charger(&p_ca->bss.cabs[cab_id]);
}

void cab_app_deactive_charge(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_deactive_charger(&p_ca->bss.cabs[cab_id]);
}

void cab_app_receive_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
	cab_cell_check_door_state(&p_ca->bss.cabs[cab_id]);
}

void cab_app_delivery_bp(Cabinet_App* p_ca, CABIN_ID cab_id){
	cab_cell_open_door(&p_ca->bss.cabs[cab_id]);
}

void cab_app_check_bp_state(Cabinet_App* p_ca, CABIN_ID cab_id){

}

void cab_app_update_tilt_ss(Cabinet_App* p_ca){

}

void cab_app_sync_bss_data_hmi(Cabinet_App* p_ca){
	char buff[50];

	bss_data_serialize(&p_ca->bss, buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_sync_bp_data_hmi(__attribute__((unused)) Cabinet_App* p_ca, BP* p_bp){
	char buff[50];

	bp_data_serialize(p_bp, buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_sync_cab_data_hmi(Cabinet_App* p_ca, uint8_t cab_id){
	char buff[50];

	cab_cell_data_serialize(&p_ca->bss.cabs[cab_id], buff);
	uart_sends(&power_sys_port, (uint8_t*)buff);
}

void cab_app_decode_cmd_hmi(Cabinet_App* p_ca, char* buff){
	char* token;

	token = strtok(buff, ",");
	switch(*token){
	case 'C':
		cab_app_process_cab_cmd_hmi(p_ca, token);
		break;
	case 'B':
		break;
	case 'S':
		break;
	default:
		break;
	}
}

void cab_app_process_bss_cmd_hmi(__attribute__((unused)) Cabinet_App* p_ca, char* token){
	token = strtok(NULL, ",");
	char* obj = token;
	token = strtok(NULL, ",");
	uint8_t state = string_to_long(token);

	switch(*obj){
	case 'F':
		break;
	case 'C':
		break;
	case 'L':
		break;
	default:
		break;
	}
}

void cab_app_process_cab_cmd_hmi(__attribute__((unused)) Cabinet_App* p_ca, char* token){
	token = strtok(NULL, ",");
	uint8_t id = string_to_long(token);
	token = strtok(NULL, ",");
	char* obj = token;
	token = strtok(NULL, ",");
	uint8_t state = string_to_long(token);

	switch(*obj){
	case 'F':
		if(state == 1){
			cab_cell_fan_turn_on(&p_ca->bss.cabs[id]);
		}
		else {
			cab_cell_fan_turn_off(&p_ca->bss.cabs[id]);
		}
		break;
	case 'D':
		if(state == 1){
			cab_cell_open_door(&p_ca->bss.cabs[id]);
		}
		break;
	case 'C':
		if(state == 1){
			cab_cell_active_charger(&p_ca->bss.cabs[id]);
		}
		else {
			cab_cell_deactive_charger(&p_ca->bss.cabs[id]);
		}
		break;
	default:
		break;
	}
}

void node_id_pin_active(uint8_t cab_id){
	switch(cab_id){
	case CAB1:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		break;
	case CAB2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		break;
	case CAB3:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		break;
	case CAB4:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		break;
	case CAB5:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		break;
	case CAB6:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case CAB7:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void node_id_pin_deactive(uint8_t cab_id){
	switch(cab_id){
	case CAB1:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		break;
	case CAB2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		break;
	case CAB3:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		break;
	case CAB4:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break;
	case CAB5:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case CAB6:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case CAB7:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}
