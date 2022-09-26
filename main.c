/*
 * main.c
 *
 *  Created on: May 24, 2021
 *      Author: KhanhDinh
 */

#include "board.h"
#include "peripheral_init.h"
#include "app_co_init.h"

//Cabinet_App selex_bss_app;
RS485_Master rs485m;

static void can_receive_handle(CAN_Hw *p_hw);
static void cab_app_update_io_cab_state(Cabinet_App*);
static void master_read_serial_number(void);
static bool master_write_object(void);
//bool sync_was;
static inline void CO_process_tpdo(CO *p_co, uint16_t time_diff_ms, bool sync_was);


static Cabinet 		bss_cabinets[CABINET_INIT];
static CO_Slave*	bp_slaves[CABINET_INIT];
static Charger 		bss_chargers[CHARGER_NUM];

static uint32_t 	sys_tick_ms = APP_STATE_MACHINE_UPDATE_TICK_mS;
static uint32_t 	com_timestamp = 0;
uint32_t 	tim2_timestamp = 0;
static uint32_t 	check_hmi_msg_timestamp = 0;
static uint8_t 		cab_id = 0;
uint8_t serial_number_var[32];
CO_Sub_Object serial_number_sobj =
	{
			.p_data = serial_number_var,	//<< Address variable receiving data
			.attr	= ODA_SDO_RW,			//<< [skip] set ODA_SDO_RW
			.len	= 32,					//<< Maximum data size that can be received
			.p_ext	= NULL					//<< [option], set NULL if not used
	};
uint8_t exam_var	= 2;        // 3 ok
CO_Sub_Object exam_tx_obj =
{
		.p_data = &exam_var,	//<< Address variable receiving data
		.attr	= ODA_SDO_RW,	//<< [skip] set ODA_SDO_RW
		.len	= 1,			//<< data size
		.p_ext	= NULL		//<< [skip] set NULL
};


void cab_app_init(Cabinet_App *p_ca) {
	p_ca->bss.cab_num = CABINET_CELL_NUM;
	p_ca->bss.charger_num = CHARGER_NUM;
	p_ca->state = CAB_APP_ST_SETUP;
	p_ca->bss.cabs = &bss_cabinets[0];
	p_ca->bss.ac_chargers = &bss_chargers[0];
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

	/* CHARGER INIT */
	bss_charger_init(&p_ca->bss);

	can_master_init((CAN_master*) p_ca, bp_slaves, CABINET_CELL_NUM, &can_port);
	can_set_receive_handle(p_ca->base.p_hw, can_receive_handle);
	can_set_read_sn_func_pointer((CAN_master*) p_ca,master_read_serial_number);
	can_set_sdo_write_obj_func_pointer((CAN_master*) p_ca,master_write_object);
	/*CANOpen Init */
	app_co_init();
}

int main(void) {
	__disable_irq();

	check_hmi_msg_timestamp = com_timestamp + CHECK_HMI_MSG_TIME_mS;
	board_init();
	cab_app_init(&selex_bss_app);

	__enable_irq();

	for (uint8_t i = 0; i < selex_bss_app.bss.cab_num; i++) {
		cab_cell_reset_io(&selex_bss_app.bss.cabs[i]);
	}
	while (1);
}
/*
void TIM2_IRQHandler(void)   //1ms
{
	sys_timestamp += sys_tick_ms;
	CO_process(&CO_DEVICE,1);
}
*/
void HAL_STATE_MACHINE_UPDATE_TICK(void)
{					//10ms /// 0.2s
	sys_timestamp += sys_tick_ms;
	switch(selex_bss_app.bss.state){
	case BSS_ST_MAINTAIN:
	case BSS_ST_ACTIVE:
		if(selex_bss_app.bss.state == BSS_ST_ACTIVE){

			cab_app_update_connected_cab_state(&selex_bss_app);
			cab_app_update_io_cab_state(&selex_bss_app);///
		}
//		CO_process(&CO_DEVICE,10);////muc uu tien cao hon while cua ham sdo download
		bss_update_cabinets_state(&selex_bss_app.bss);
//		can_master_process((CAN_master*) &selex_bss_app, sys_timestamp); /// se bi thay the boi ham CO_process
		can_master_update_id_assign_process((CAN_master*) &selex_bss_app, sys_timestamp);

			bool sync_was;
			static bool tpdo_send_req = false;

			sync_was = CO_SYNC_process(&CO_DEVICE.sync, 1, 10);
			for(int i=0;i<1000;i++);
//			CO_SDOserver_process(&p_co->sdo_server, time_diff_ms);
			CO_SDOclient_process(&CO_DEVICE.sdo_client, 10);

//			CO_process_tpdo(&CO_DEVICE, 10, tpdo_send_req);
//			tpdo_send_req = false;
//			if(true == sync_was)
//			{
//				tpdo_send_req = true;
//			}


		break;
	case BSS_ST_INIT:
	case BSS_ST_FAIL:
		break;
	}
	cab_app_process_hmi_command(&selex_bss_app, sys_timestamp);
}

void TIM3_IRQHandler(void) { //// 1ms
	com_timestamp += sys_tick_ms;

	/* Process RS485 Protocol */
	rs485_master_update_state(&rs485m, com_timestamp);

	/* Process HMI protocol */
	if(check_hmi_msg_timestamp == com_timestamp){
		check_hmi_msg_timestamp = com_timestamp + CHECK_HMI_MSG_TIME_mS;
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

	/*CO_process*/
		CO_process(&CO_DEVICE,1);

#if ENABLE_IWDG_TIMER
	HAL_IWDG_Refresh(&hiwdg);
#endif

	HAL_TIM_IRQHandler(&io_scan_timer);
}

/* ------------------------------------------------------------------------------ */

static void can_receive_handle(CAN_Hw *p_hw)
{
#if 0
	app_co_can_receive_handle(p_can_hw->RxHeader.Identifier, p_can_hw->rx_msg_data);
#endif
	uint32_t cob_id = p_hw->can_rx.StdId;

	switch(p_hw->can_rx.StdId & 0xFFFFFF80)
	{
		case CO_CAN_ID_TPDO_1:
		case CO_CAN_ID_TPDO_2:
		case CO_CAN_ID_TPDO_3:
		case CO_CAN_ID_TPDO_4:
		selex_bss_app.base.rpdo_process((CAN_master*)&selex_bss_app);
			break;
		default:
			break;
	}
	CO_can_receive_basic_handle(&CO_DEVICE, cob_id, p_hw->rx_data);
	/* if assign request message */
	if (cob_id == selex_bss_app.base.node_id_scan_cobid) // bp send can_id = 0x70
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
			/* slave confirm assign id success*/
			if (p_hw->rx_data[0] != selex_bss_app.base.assigning_slave->node_id) return;
			cm_start_authorize_slave((CAN_master*) &selex_bss_app,selex_bss_app.base.assigning_slave, sys_timestamp); // den day
		}
		return;
	}

//	if (cob_id == selex_bss_app.base.sdo_server.rx_address)
//	{
//		CO_memcpy(selex_bss_app.base.sdo_server.rx_msg_data, p_hw->rx_data, 8);
//		selex_bss_app.base.sdo_server.is_new_msg = 1;
//		HAL_CAN_DISABLE_IRQ;
//	}
}

static void cab_app_update_io_cab_state(Cabinet_App* p_app)
{
	if(cab_id == p_app->bss.cab_num)
	{
		cab_id = 0;
	}
	cab_cell_update_io_state(&p_app->bss.cabs[cab_id]);
	cab_id++;

#if ENABLE_CHARGER
	cab_app_update_charge(p_app, sys_timestamp);
#endif
}
static void master_read_serial_number()
{
//	CO_SDO_reset_status(&CO_DEVICE.sdo_client);
	CO_SDOclient_start_upload(&CO_DEVICE.sdo_client, 5, 0x2003, 0x00, &serial_number_sobj, 2000);
}
static bool master_write_object()
{
	CO_SDO* p_sdo = &CO_DEVICE.sdo_client;
	// Waiting status SDOclient = idle
//	while(CO_SDO_get_status(p_sdo) != CO_SDO_RT_idle);

	// Start command
	CO_SDOclient_start_download(&CO_DEVICE.sdo_client, 5, 0x2003, 0x01, &exam_tx_obj, 2000);

	// Waiting process is complete
//	while(CO_SDO_get_status(p_sdo) == CO_SDO_RT_busy);
	if(CO_SDO_get_status(p_sdo) == CO_SDO_RT_success)
	{
		return 1; //success
	}
	else
	{
		return 0; //fail
	}
}
static inline void CO_process_tpdo(CO *p_co, uint16_t time_diff_ms, bool sync_was)
{
    for (uint8_t i = 0; i < TPDO_NUMBER; i++)
    {
        CO_TPDO_process(&p_co->tpdos[i],time_diff_ms, sync_was);
    }
}
