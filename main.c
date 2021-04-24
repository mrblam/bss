/*
 * test_com_hmi.c
 *
 *  Created on: Apr 21, 2021
 *      Author: Ha
 */

/*
 * test_sync_data.c
 *
 *  Created on: Apr 15, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "cabinet_app.h"

BP* bp_test;
static volatile uint8_t sync_counter = 20;
static volatile uint8_t cab_id = 0;
char buff[50];
char s;
uint8_t idx, cnt, i, char_state, get_cmd_done;

Cabinet_App selex_bss_app;
static void cabinet_door_close_event_handle(Cabinet* p_cab);
static void can_master_slave_select_impl(const CAN_master* p_cm,const uint32_t id);
static void can_master_slave_deselect_impl(const CAN_master* p_cm,const uint32_t id);
static void bp_assign_id_success_handle(const CAN_master* const p_cm,const uint32_t id);
static void bp_assign_id_fail_handle(const CAN_master* const p_cm,const uint32_t id);


static Cabinet bss_cabinets[CABINET_CELL_NUM];
static CO_Slave* bp_slaves[CABINET_CELL_NUM];
static uint32_t sys_timestamp=0;
static uint32_t sys_tick_ms=0;

void cab_app_init(Cabinet_App* p_ca){
	p_ca->state = CABIN_ST_SETUP;
	p_ca->bss.cabs=&bss_cabinets[0];
	for(int i=0;i<CABINET_CELL_NUM;i++){
	        bss_cabinets[i].cab_id=i;
	        bss_cabinets[i].bp=(BP*)malloc(sizeof(BP));
	        bss_cabinets[i].on_door_close=cabinet_door_close_event_handle;
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
	p_ca->base.slave_deselect=can_master_slave_deselect_impl;
	p_ca->base.on_slave_assign_fail=bp_assign_id_fail_handle;
	p_ca->base.on_slave_assign_success=bp_assign_id_success_handle;

	p_ca->ioe_cfan = ioe_construct();
	p_ca->ioe_sol = ioe_construct();
}


int main (void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss_app);
	cab_app_set_state(&selex_bss_app, CABIN_ST_STANDBY);
	uart_receives(&power_sys_port, &s);
	sys_tick_ms=1000/SYSTICK_FREQ_Hz;
	sys_timestamp=0;
	__enable_irq();
	while(1){
	};
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

	sys_timestamp+= sys_tick_ms;
	bss_update_cabinets_state(&selex_bss_app.bss);
	can_master_process((CAN_master*)&selex_bss_app, sys_timestamp);
	can_master_update_id_assign_process((CAN_master*)&selex_bss_app,sys_timestamp);
}

void USART1_IRQHandler(void){
	HAL_CHECK_COM_IRQ_REQUEST(&power_sys_port.uart_module);

	switch(char_state){
	case 0:
		if(s == ':'){
			char_state = 1;
		}
		break;
	case 1:
		if(s == 'W'){
			char_state = 2;
		}
		break;
	case 2:
		if(s == '*'){
			char_state = idx = 0;
			get_cmd_done = 1;
			break;
		}
		*(buff + idx) = s;
		idx++;
		break;
	default:
		break;
	}

	uart_receives(&power_sys_port, &s);
}

void HAL_HMI_PROCESS_DATA_IRQ(void){
	CHECK_TIM_IRQ_REQUEST(&hmi_timer);

	if(sync_counter < 20){
		sync_counter++;
	}
	else{
		switch(sync_counter){
		case 20:
			if(cab_id < CABINET_CELL_NUM){
				cab_app_sync_cab_data_hmi(&selex_bss_app, cab_id);
				cab_id++;
			}
			else sync_counter = 21;
			break;
		case 21:
			/*
			if(cab_list_walk_down(selex_bss_app.bss.empty_cabs)){
				cab_app_sync_bp_data_hmi(&selex_bss_app, selex_bss_app.bss.empty_cabs->p_temp->data->bp);
			}
			else sync_counter = 22;
			*/
			break;
		case 22:
			cab_app_sync_bss_data_hmi(&selex_bss_app);
			sync_counter = 20;
			cab_id = 0;
			break;
		default:
			break;
		}
	}

	if(get_cmd_done == 1){
		cab_app_decode_cmd_hmi(&selex_bss_app, buff);
		get_cmd_done = 0;
	}
}

static void cabinet_door_close_event_handle(Cabinet* p_cab){
	can_master_slave_select((CAN_master*)&selex_bss_app, p_cab->cab_id);
}

static void can_master_slave_select_impl(const CAN_master* p_cm,const uint32_t id){
	(void)p_cm;
	sw_on(selex_bss_app.bss.cabs[id].node_id_sw);
}

static void can_master_slave_deselect_impl(const CAN_master* p_cm,const uint32_t id){
	(void)p_cm;
	sw_off(selex_bss_app.bss.cabs[id].node_id_sw);
}

static void bp_assign_id_success_handle(const CAN_master* const p_cm,const uint32_t id){


}

static void bp_assign_id_fail_handle(const CAN_master* const p_cm,const uint32_t id){

}
