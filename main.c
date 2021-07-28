/*
 * main.c
 *
 *  Created on: May 24, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"
#include "board.h"
#include "peripheral_init.h"

Cabinet_App selex_bss_app;
RS485_Master rs485m;

static void cabinet_door_close_event_handle(Cabinet *p_cab);
static void cabinet_door_open_event_handle(Cabinet *p_cab);
static void can_master_slave_select_impl(const CAN_master *p_cm, const uint32_t id);
static void can_master_slave_deselect_impl(const CAN_master *p_cm, const uint32_t id);
static void bp_assign_id_success_handle(const CAN_master *const p_cm, const uint32_t id);
static void bp_assign_id_fail_handle(const CAN_master *const p_cm, const uint32_t id);
static void can_receive_handle(CAN_Hw *p_hw);

static Cabinet bss_cabinets[CABINET_CELL_NUM];
static CO_Slave *bp_slaves[CABINET_CELL_NUM];
static Charger bss_chargers[CHARGER_NUM];
static uint8_t	id_assign_cabs_charger[CHARGER_NUM][MAX_ASSIGNED_CABINET] = {{0,3,4,7,8,11,12,15,16}, {1,2,5,6,9,10,13,14,17,18}};

static uint32_t sys_timestamp = 0;
static uint32_t sys_tick_ms = 0;
static uint32_t com_timestamp = 0;
static uint32_t check_hmi_msg_timestamp = 0;
static uint8_t cab_id = 0;

void cab_app_init(Cabinet_App *p_ca) {
	p_ca->bss.cab_num = CABINET_CELL_NUM;
	p_ca->bss.charger_num = CHARGER_NUM;
	p_ca->state = CAB_APP_ST_SETUP;
	p_ca->bss.cabs = &bss_cabinets[0];
	p_ca->bss.ac_chargers = &bss_chargers[0];
	bss_init(&p_ca->bss);
	peripheral_init(p_ca);
	for (int i = 0; i < p_ca->bss.cab_num; i++) {
		bss_cabinets[i].op_state = CAB_CELL_ST_INIT;
		bss_cabinets[i].cab_id = i;
		bss_cabinets[i].bp = bp_construct();
		bss_cabinets[i].bp->pos = i;
		bss_cabinets[i].bp->vol = 0;
		bss_cabinets[i].bp->cur = 0;
		bss_cabinets[i].bp->soc = 90;
		bss_cabinets[i].bp->soc = 100;
		bss_cabinets[i].bp->cycle = 8;
		bss_cabinets[i].bp->state = BP_ST_INIT;
		for (uint8_t j = 0; j < 8; j++)	bss_cabinets[i].bp->temp[j] = 26;
		bss_cabinets[i].on_door_close = cabinet_door_close_event_handle;
		bss_cabinets[i].on_door_open = cabinet_door_open_event_handle;
		bss_cabinets[i].is_changed = 0;
		cab_cell_init(&bss_cabinets[i]);
		while (bss_cabinets[i].bp == NULL);
		bp_slaves[i] = (CO_Slave*) (bss_cabinets[i].bp);
		co_slave_set_con_state(bp_slaves[i], CO_SLAVE_CON_ST_DISCONNECT);
		bp_slaves[i]->node_id = CABINET_START_NODE_ID + i;
		bp_slaves[i]->sdo_server_address = 0x580 + bp_slaves[i]->node_id;
		bp_slaves[i]->inactive_time_ms = 0;
	}

	/* CHARGER INIT */
	for(uint8_t i = 0; i < p_ca->bss.charger_num; i++){
		p_ca->bss.ac_chargers[i].charging_cabin = NULL;
		p_ca->bss.ac_chargers[i].input_power.state = SW_ST_OFF;
		for(uint8_t j = 0; j < p_ca->bss.ac_chargers[i].assigned_cab_num; j++){
			p_ca->bss.ac_chargers[i].assigned_cabs[j] = &bss_cabinets[id_assign_cabs_charger[i][j]];
		}
	}

	p_ca->base.slave_start_node_id = CABINET_START_NODE_ID;
	can_master_init((CAN_master*) p_ca, bp_slaves, CABINET_CELL_NUM, &can_port);
	p_ca->base.assign_state = CM_ASSIGN_ST_DONE;
	p_ca->base.slave_select = can_master_slave_select_impl;
	p_ca->base.slave_deselect = can_master_slave_deselect_impl;
	p_ca->base.on_slave_assign_fail = bp_assign_id_fail_handle;
	p_ca->base.on_slave_assign_success = bp_assign_id_success_handle;
	p_ca->base.sdo_server.rx_address = 0x600 + 3;

	can_set_receive_handle(p_ca->base.p_hw, can_receive_handle);
}

int main(void) {
	__disable_irq();

	board_init();

	HAL_MASTER_LED4_SET_HIGH;
	HAL_MASTER_LED1_SET_HIGH;
	HAL_MASTER_LED1_SET_LOW;
	HAL_MASTER_LED2_SET_HIGH;
	HAL_MASTER_LED2_SET_LOW;
	HAL_MASTER_LED3_SET_HIGH;
	HAL_MASTER_LED3_SET_LOW;


	sys_tick_ms = 1000 / SYSTICK_FREQ_Hz;
	sys_timestamp = 0;
	check_hmi_msg_timestamp = com_timestamp + 200;

	cab_app_init(&selex_bss_app);
	cab_app_set_state(&selex_bss_app, CAB_APP_ST_SETUP);

	__enable_irq();

	for (uint8_t i = 0; i < selex_bss_app.bss.cab_num; i++) {
		sw_off(&selex_bss_app.bss.cabs[i].node_id_sw);
		sw_off(&selex_bss_app.bss.cabs[i].charger);
	}

	while (1);
}

void HAL_STATE_MACHINE_UPDATE_TICK(void) {
	sys_timestamp += sys_tick_ms;

	switch(selex_bss_app.bss.state){
	case BSS_ST_INIT:
		break;
	case BSS_ST_MAINTAIN:
	case BSS_ST_ACTIVE:
		if(selex_bss_app.bss.state == BSS_ST_ACTIVE){
#if 0
			for(uint8_t id = 0; id < selex_bss_app.bss.cab_num; id++){
				if((selex_bss_app.bss.cabs[id].bp->base.con_state == CO_SLAVE_CON_ST_CONNECTED) &&
						(selex_bss_app.base.pdo_sync_timestamp)){

					selex_bss_app.bss.cabs[id].bp->base.inactive_time_ms += sys_tick_ms;

					if(selex_bss_app.bss.cabs[id].bp->base.inactive_time_ms > BP_INACTIVE_TIMEOUT_mS){
						cab_cell_reset(&selex_bss_app.bss.cabs[id]);
						sdo_server_set_state(&selex_bss_app.base.sdo_server, SDO_ST_IDLE);
					}
				}
			}
#endif
			if(cab_id == selex_bss_app.bss.cab_num){
				cab_id = 0;
			}
			cab_cell_update_io_state(&selex_bss_app.bss.cabs[cab_id]);
			cab_id++;
		}

		bss_update_cabinets_state(&selex_bss_app.bss);
		can_master_process((CAN_master*) &selex_bss_app, sys_timestamp);
		can_master_update_id_assign_process((CAN_master*) &selex_bss_app, sys_timestamp);
		break;
	case BSS_ST_FAIL:
	default:
		break;
	}

	cab_app_process_hmi_command(&selex_bss_app, sys_timestamp);
}

void TIM3_IRQHandler(void) {
	com_timestamp += 10;

	/* Process RS485 Protocol */
	rs485_master_update_state(&rs485m, com_timestamp);

	/* Process HMI protocol */
	if(check_hmi_msg_timestamp == com_timestamp){
		check_hmi_msg_timestamp = com_timestamp + 200;
		if(selex_bss_app.is_new_msg){
			cab_app_check_buffer(&selex_bss_app);
		}
	}
	/* Sync Data to HMI */
	for(uint8_t i = 0; i < selex_bss_app.hmi_csv.valid_msg_num; i++){
		if(selex_bss_app.hmi_csv.is_new_msg_to_send[i]){
			cab_app_send_msg_to_hmi(&selex_bss_app);
			selex_bss_app.hmi_csv.is_new_msg_to_send[i] = 0;
		}
	}

	HAL_TIM_IRQHandler(&io_scan_timer);
}

/* ------------------------------------------------------------------------------ */

static void can_receive_handle(CAN_Hw *p_hw) {
	uint32_t cob_id = p_hw->can_rx.StdId;

	switch(p_hw->can_rx.StdId & 0xFFFFFF80){
	case CO_CAN_ID_TPDO_1:
	case CO_CAN_ID_TPDO_2:
	case CO_CAN_ID_TPDO_3:
	case CO_CAN_ID_TPDO_4:
		selex_bss_app.base.rpdo_process((CAN_master*)&selex_bss_app);
		break;
	default:
		break;
	}

	/* if assign request message */
	if (cob_id == selex_bss_app.base.node_id_scan_cobid) {
		if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_WAIT_REQUEST) {
			selex_bss_app.base.p_hw->can_tx.StdId = selex_bss_app.base.node_id_scan_cobid;
			selex_bss_app.base.p_hw->can_tx.DLC = 0;
			can_send(selex_bss_app.base.p_hw, selex_bss_app.base.p_hw->tx_data);
			selex_bss_app.base.assign_state = CM_ASSIGN_ST_START;

		}
		else if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_SLAVE_SELECT) {
			selex_bss_app.base.assign_state = CM_ASSIGN_ST_SLAVE_SELECT_CONFIRM;

		}
		else if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_WAIT_CONFIRM) {
			/* slave confirm assign id success*/
			if (p_hw->rx_data[0] == selex_bss_app.base.assigning_slave->node_id) {
				/* finish assign for current slave and move to next slave */
				cm_start_authorize_slave((CAN_master*) &selex_bss_app, selex_bss_app.base.assigning_slave);
			}
		}
	}

	if (cob_id == selex_bss_app.base.sdo_server.rx_address) {
		CO_memcpy(selex_bss_app.base.sdo_server.rx_msg_data, p_hw->rx_data, 8);
		selex_bss_app.base.sdo_server.is_new_msg = 1;
	}
}

static void cabinet_door_close_event_handle(Cabinet *p_cab) {
	(void)p_cab;
	if (selex_bss_app.state == CAB_APP_ST_SETUP) return;

	//bp_set_con_state(p_cab->bp, CO_SLAVE_CON_ST_ASSIGNING);
	//can_master_start_assign_next_slave((CAN_master*) &selex_bss_app, sys_timestamp);
}

static void cabinet_door_open_event_handle(Cabinet *p_cab) {
	/* ignore event during setup process*/
	if (selex_bss_app.state == CAB_APP_ST_SETUP) return;

	bp_set_con_state(p_cab->bp, CO_SLAVE_CON_ST_DISCONNECT);
	cab_cell_update_state(p_cab);
	sw_off(&p_cab->node_id_sw);
}

static void can_master_slave_select_impl(const CAN_master *p_cm, const uint32_t id) {
	(void) p_cm;
	sw_off(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void can_master_slave_deselect_impl(const CAN_master *p_cm, const uint32_t id) {
	(void) p_cm;
	sw_on(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void bp_assign_id_success_handle(const CAN_master *const p_cm, const uint32_t id) {
	(void) p_cm;
	sw_on(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void bp_assign_id_fail_handle(const CAN_master *const p_cm, const uint32_t id) {
	(void) p_cm;
	/* return battery to user */
	cab_cell_update_state(&selex_bss_app.bss.cabs[id]);
	sw_off(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

