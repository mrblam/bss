/*
 * main.c
 *
 *  Created on: May 24, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "peripheral_init.h"
#include "app_co_init.h"
#include "bootloader_app.h"
#include "host_master.h"


RS485_Master rs485m;
char s[100] = "hoanpx";
static void can_receive_handle(CAN_Hw *p_hw);
static void cab_app_update_io_cab_state(Cabinet_App*);

static Cabinet 		bss_cabinets[CABINET_INIT];
static CO_Slave*	bp_slaves[CABINET_INIT];
static Charger 		bss_chargers[CHARGER_NUM];
static uint32_t 	sys_tick_ms = APP_STATE_MACHINE_UPDATE_TICK_mS;
static uint32_t 	com_timestamp = 0;
static uint32_t 	tim2_timestamp = 0;
static uint32_t 	check_hmi_msg_timestamp = 0;
static uint8_t 		cab_id = 0;
static uint8_t		node_id_high = 0;
int32_t my_callback(int32_t _cmd, const uint8_t* _data, int32_t _len, void* _arg ){
	can_port.can_tx.StdId = ((uint16_t)_data[0] << 8) | (uint16_t)_data[1];
	can_port.can_tx.DLC = _len - 2;
	can_send(&can_port, (uint8_t*)(_data + 2));
	selex_bss_app.upgrade_timeout = sys_timestamp + 5000;
	return _len;
}
int32_t my_send_interface(const uint8_t* _data, int32_t _len){
	HAL_UART_Transmit(&hmi_com.uart_module, (uint8_t*)_data, _len, 500);
	return _len;
}
sm_host_t* host_master;

void cab_app_init(Cabinet_App *p_ca) {
	p_ca->bss.cab_num = CABINET_CELL_NUM;
	p_ca->bss.charger_num = CHARGER_NUM;
	p_ca->state = CAB_APP_ST_SETUP;
	p_ca->bss.cabs = &bss_cabinets[0];
	p_ca->bss.ac_chargers = &bss_chargers[0];
	p_ca->bss.ac_meter.rx_index = 0;
	p_ca->is_main_hmi_shutdown = false;
	bss_init(&p_ca->bss);
	peripheral_init(p_ca);
	for (int i = 0; i < CABINET_INIT; i++) {
		cab_cell_init(&bss_cabinets[i], i);
		bp_slaves[i] = (CO_Slave*) (bss_cabinets[i].bp);
		co_slave_set_con_state(bp_slaves[i], CO_SLAVE_CON_ST_DISCONNECT);
		bp_slaves[i]->node_id = CABINET_START_NODE_ID + i;
		bp_slaves[i]->sdo_server_address = 0x580 + bp_slaves[i]->node_id;
		bp_slaves[i]->inactive_time_ms = 0;
	}
	bss_charger_init(&p_ca->bss);
	can_master_init((CAN_master*) p_ca, bp_slaves, CABINET_CELL_NUM, &can_port);
	can_set_receive_handle(p_ca->base.p_hw, can_receive_handle);
	app_co_init();
}
int main(void) {
	__disable_irq();
	check_hmi_msg_timestamp = com_timestamp + CHECK_HMI_MSG_TIME_mS;
	board_init();
	cab_app_init(&selex_bss_app);
	sm_host_cmd_callback_fn_t my_callback_ptr = my_callback;
	sm_host_send_if my_send_interface_ptr = my_send_interface;
	host_master = sm_host_create(SM_HOST_ADDR_DEFAULT,my_send_interface_ptr);
	sm_host_reg_handle(host_master,my_callback_ptr,NULL);
	__enable_irq();
	for (uint8_t i = 0; i < selex_bss_app.bss.cab_num; i++) {
		cab_cell_reset_io(&selex_bss_app.bss.cabs[i]);
	}
	while (1){
		switch (selex_bss_app.bss.state) {
		case BSS_ST_MAINTAIN:
			bss_update_cabinets_state(&selex_bss_app.bss);
			can_master_update_id_assign_process((CAN_master*) &selex_bss_app,sys_timestamp);
			break;
		case BSS_ST_ACTIVE:
			cab_app_update_connected_cab_state(&selex_bss_app);
			cab_app_update_io_cab_state(&selex_bss_app);
			bss_update_cabinets_state(&selex_bss_app.bss);
			can_master_update_id_assign_process((CAN_master*) &selex_bss_app,sys_timestamp);
			break;
		case BSS_ST_UPGRADE_FW_BP:
			sm_host_process(host_master);
			if(selex_bss_app.upgrade_timeout < sys_timestamp
				&& selex_bss_app.base.sdo_service == SDO_SERVICE_BOOT_BMS ){
				selex_bss_app.is_main_hmi_shutdown = false;
				selex_bss_app.bss.state = BSS_ST_ACTIVE;
				selex_bss_app.base.sdo_service = SDO_SERVICE_IDLE;
				uart_hw_init();
			}
			break;
		case BSS_ST_INIT:
		case BSS_ST_FAIL:
			break;
		}
		if(selex_bss_app.is_new_msg){
			cab_app_check_buffer(&selex_bss_app);
		}
		cab_app_process_hmi_command(&selex_bss_app, sys_timestamp, &sys_timestamp);
	}
}
void TIM2_IRQHandler(void)
{
	tim2_timestamp ++;
	if(node_id_high){
		node_id_high = 0;
		can_master_slave_deselect(&selex_bss_app.base,selex_bss_app.base.assigning_slave->node_id - selex_bss_app.base.slave_start_node_id);
	}
	if(selex_bss_app.base.sdo_service != SDO_SERVICE_BOOT_BMS){
		CO_process(&CO_DEVICE,1);
	}
	if(selex_bss_app.base.sdo_service == SDO_SERVICE_ACTIVE_CHARGER || selex_bss_app.base.sdo_service == SDO_SERVICE_DEACTIVE_CHARGER){
		if (selex_bss_app.base.CO_base.sdo_client.state == CO_SDO_ST_IDLE && selex_bss_app.base.CO_base.sdo_client.status == CO_SDO_RT_success){
			selex_bss_app.base.CO_base.sdo_client.status = CO_SDO_RT_idle;
			selex_bss_app.base.sdo_service = SDO_SERVICE_IDLE;
		}
	}
	can_master_update_sn_assign_process((CAN_master*) &selex_bss_app);
	HAL_TIM_IRQHandler(&hmi_timer);
}

void HAL_STATE_MACHINE_UPDATE_TICK(void)
{
	sys_timestamp += sys_tick_ms;
	if(selex_bss_app.slave_com->state == RS485_MASTER_ST_MOBUS){
		if(sys_timestamp > selex_bss_app.bss.ac_meter.timeout){
			selex_bss_app.bss.ac_meter.finish_read = true;
		}
	}
	if(selex_bss_app.base.sdo_service == SDO_SERVICE_REQ_BOOT_BMS){
		if(sys_timestamp > selex_bss_app.base.sdo_timeout){
			selex_bss_app.base.sdo_finish = true;
		}
	}
}

/* --------------------------------------------------------------------------------------- */

void TIM3_IRQHandler(void) {
	com_timestamp += 5;
	if(selex_bss_app.slave_com->state != RS485_MASTER_ST_MOBUS){
		rs485_master_update_state(&rs485m, com_timestamp);
	}
#if ENABLE_IWDG_TIMER
	HAL_IWDG_Refresh(&hiwdg);
#endif

	HAL_TIM_IRQHandler(&io_scan_timer);
}

/* ------------------------------------------------------------------------------ */

static void can_receive_handle(CAN_Hw *p_hw){
#if 0
	app_co_can_receive_handle(p_can_hw->RxHeader.Identifier, p_can_hw->rx_msg_data);
#else
	uint32_t cob_id = p_hw->can_rx.StdId;
	if (selex_bss_app.is_main_hmi_shutdown == true) {
		uint8_t array[32];
		uint8_t len = 0;
		array[len++] = (cob_id >> 8) & 0xFF;
		array[len++] = (cob_id) & 0xFF;
		for (uint32_t i = 0; i < p_hw->can_rx.DLC; i++) {
			array[len++] = p_hw->rx_data[i];
		}
		sm_host_send_response(host_master, 0x10, 0x00, array, len);
//		return;
	}
	switch(p_hw->can_rx.StdId & 0xFFFFFF80)
	{
		case CO_CAN_ID_TPDO_1:
		case CO_CAN_ID_TPDO_2:
		case CO_CAN_ID_TPDO_3:
		case CO_CAN_ID_TPDO_4:
		case CO_CAN_ID_RPDO_1:
		case CO_CAN_ID_RPDO_2:
		selex_bss_app.base.rpdo_process((CAN_master*)&selex_bss_app);
			break;
		default:
			break;
	}
	CO_can_receive_basic_handle(&CO_DEVICE, cob_id, p_hw->rx_data);
	if (cob_id == selex_bss_app.base.node_id_scan_cobid)
	{
		if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_WAIT_REQUEST)
		{
			selex_bss_app.base.p_hw->can_tx.StdId = selex_bss_app.base.node_id_scan_cobid;
			selex_bss_app.base.p_hw->can_tx.DLC = 0;
			can_send(selex_bss_app.base.p_hw, selex_bss_app.base.p_hw->tx_data);
			selex_bss_app.base.assign_state = CM_ASSIGN_ST_START;
		}
		else if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_SLAVE_SELECT)
		{
			selex_bss_app.base.assign_state = CM_ASSIGN_ST_SLAVE_SELECT_CONFIRM;
		}
		else if (selex_bss_app.base.assign_state == CM_ASSIGN_ST_WAIT_CONFIRM)
		{
			selex_bss_app.bss.cabs[selex_bss_app.base.assigning_slave->node_id - selex_bss_app.base.slave_start_node_id].bp->base.inactive_time_ms = 10000 + sys_timestamp;
			if (p_hw->rx_data[0] != selex_bss_app.base.assigning_slave->node_id) return;
			cm_start_authorize_slave((CAN_master*) &selex_bss_app,selex_bss_app.base.assigning_slave, sys_timestamp);
			node_id_high = 1;
		}
		return;
	}
#endif
}

static void cab_app_update_io_cab_state(Cabinet_App* p_app)
{
	if (cab_id == p_app->bss.cab_num) {
		cab_id = 0;
	}
	cab_cell_update_io_state(&p_app->bss.cabs[cab_id]);
	p_app->bss.cabs[cab_id].assign_state = p_app->base.assign_state;
	cab_id++;

#if ENABLE_CHARGER
	cab_app_update_charge(p_app);
#endif
//	if(sys_timestamp % 5000 == 0){
//	p_app->bss.bp_backup.get_voltage(&p_app->bss.bp_backup);
//	}
}
