/*
 * can_master.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"

uint8_t reassign_attemp_cnt = 0;
static int delay_ms = 0;

static CO_Slave* can_master_get_assign_request_slave(const CAN_master *const p_cm);
static void can_master_process_sdo(CAN_master *p_cm, const uint32_t timestamp);
static void delay_time_ms(int time_ms);

void can_master_init(CAN_master *p_cm, CO_Slave **slaves,
		const uint32_t slave_num, CAN_Hw *p_hw) {
	p_cm->node_id_scan_cobid = CAN_NODE_ID_ASSIGN_COBID;
	p_cm->slave_start_node_id = CABINET_START_NODE_ID;
	p_cm->slaves = slaves;
	p_cm->p_hw = p_hw;
	p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
	p_cm->sdo_server.state = SDO_ST_IDLE;
	p_cm->sdo_server.is_new_msg = 0;
	p_cm->slave_num = slave_num;
	p_cm->assign_state = CM_ASSIGN_ST_DONE;
}

void can_master_start_assign_slave(CAN_master *p_cm, CO_Slave *slave,
		const uint32_t timestamp) {
	co_slave_set_con_state(slave, CO_SLAVE_CON_ST_ASSIGNING);
	p_cm->assigning_slave = slave;
	can_master_slave_select(p_cm,p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
	for (uint8_t i = 0; i < 32; i++) {
		p_cm->assigning_slave->sn[i] = 0;
	}
	p_cm->assign_timeout = timestamp + WAIT_BP_ASSIGN_REQ_TIMEOUT_mS;
	p_cm->assign_state = CM_ASSIGN_ST_WAIT_REQUEST;
	can_master_slave_deselect(p_cm,p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
}

void cm_start_authorize_slave(CAN_master *p_cm, CO_Slave *slave, uint32_t timestamp) {
	p_cm->assign_state = CM_ASSIGN_ST_AUTHORIZING;
	p_cm->authorizing_timeout = timestamp + 10000;
}
void can_set_read_sn_func_pointer(CAN_master *p_cm,void (*read_serial_number_bp)(void)) {
	p_cm->read_serial_number_bp = read_serial_number_bp;
}
void can_master_read_slave_sn(CAN_master *p_cm, uint8_t cab_id,uint32_t timestamp) {
	p_cm->serial_number_sobj.attr = ODA_SDO_RW;
	p_cm->serial_number_sobj.p_data = p_cm->slaves[cab_id]->sn;
	p_cm->serial_number_sobj.len = 32;
	p_cm->serial_number_sobj.p_ext = NULL;
	CO_SDOclient_start_upload(&p_cm->CO_base.sdo_client,
								p_cm->slaves[cab_id]->node_id,
								BMS_INDEX,
								BMS_SERIAL_NUMBER_OBJECT_SUB_INDEX,
								&p_cm->serial_number_sobj,
								SDO_READ_OBJ_TIMEOUT_mS);
	p_cm->sdo_server.node_id_processing = p_cm->slaves[cab_id]->node_id;
}
void can_master_write_bms_object(CAN_master *p_cm, uint8_t cab_id,
		BMS_OBJ bms_obj) {
	switch (bms_obj) {
	case BMS_MAINSWITCH:
		p_cm->data_write_bms_od.p_data = &p_cm->bms_mainswitch_state;
		p_cm->data_write_bms_od.attr = ODA_SDO_RW;
		p_cm->data_write_bms_od.len = 1;
		p_cm->data_write_bms_od.p_ext = NULL;
		CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
				p_cm->slaves[cab_id]->node_id,
				BMS_INDEX,
				BMS_MAINSWITCH_SUB_INDEX, &p_cm->data_write_bms_od,
				SDO_WRITE_OBJ_TIMEOUT_mS);
		p_cm->sdo_server.node_id_processing = p_cm->slaves[cab_id]->node_id;

		break;
	case BMS_MATTING:
		p_cm->data_write_bms_od.p_data = &p_cm->bms_matting_state;
		p_cm->data_write_bms_od.attr = ODA_SDO_RW;
		p_cm->data_write_bms_od.len = 1;
		p_cm->data_write_bms_od.p_ext = NULL;
		CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
				p_cm->slaves[cab_id]->node_id, 0x2004, 0x00,
				&p_cm->data_write_bms_od,
				SDO_WRITE_OBJ_TIMEOUT_mS);

		break;
	case BMS_MATED_DEV:
		p_cm->sn_assign_state = BMS_MATED_DEV_WRITE_BSS_SN;
		p_cm->slave_id = cab_id;
		break;
	case BMS_NEW_FW_REQ:
		p_cm->data_write_bms_od.p_data = &p_cm->bms_new_fw_req;
		p_cm->data_write_bms_od.attr = ODA_SDO_RW;
		p_cm->data_write_bms_od.len = 1;
		p_cm->data_write_bms_od.p_ext = NULL;
		CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
									p_cm->slaves[cab_id]->node_id,
									0x2001,
									0x07,
									&p_cm->data_write_bms_od,
									SDO_WRITE_OBJ_TIMEOUT_mS);

		break;
	default:
		break;
	}
}
void can_master_update_id_assign_process(CAN_master *p_cm,const uint32_t timestamp) {
	static uint8_t retry_read_sn_bp = 0;
	switch (p_cm->assign_state) {
	case CM_ASSIGN_ST_WAIT_REQUEST:
	case CM_ASSIGN_ST_SLAVE_SELECT:
	case CM_ASSIGN_ST_WAIT_CONFIRM:
		if (p_cm->assign_timeout < timestamp) {
			p_cm->assign_state = CM_ASSIGN_ST_FAIL;
		}
		break;
	case CM_ASSIGN_ST_START:
		p_cm->assign_state = CM_ASSIGN_ST_SLAVE_SELECT;
		can_master_slave_select(p_cm,
				p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
		break;
	case CM_ASSIGN_ST_SLAVE_SELECT_CONFIRM:
		p_cm->assign_state = CM_ASSIGN_ST_WAIT_CONFIRM;
		p_cm->p_hw->can_tx.StdId = p_cm->node_id_scan_cobid;
		p_cm->p_hw->can_tx.DLC = 1;
		p_cm->p_hw->tx_data[0] = p_cm->assigning_slave->node_id;
		can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
		p_cm->assign_timeout = timestamp + SLAVE_SELECT_CONFIRM_TIMEOUT_mS;
		break;
	case CM_ASSIGN_ST_AUTHORIZING:
		if (p_cm->authorizing_timeout < timestamp) {
			p_cm->assign_state = CM_ASSIGN_ST_FAIL;
		}
		can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
		if(p_cm->CO_base.sdo_client.status == CO_SDO_RT_idle){
			can_master_read_slave_sn(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id, timestamp);
		}
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort) {
			if(retry_read_sn_bp > 5){
				p_cm->assign_state = CM_ASSIGN_ST_FAIL;
				retry_read_sn_bp = 0;
			}
			retry_read_sn_bp ++;
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
		} else if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_success) {
			retry_read_sn_bp = 0;
			co_slave_set_con_state(p_cm->assigning_slave, CO_SLAVE_CON_ST_AUTHORIZING);
			p_cm->on_slave_assign_success(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
			reassign_attemp_cnt = 0;
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			can_master_read_bp_version_software(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
		}
		break;
	case CM_ASSIGN_ST_DONE:
		if(p_cm->CO_base.sdo_client.status == CO_SDO_RT_success && p_cm->sdo_service == SDO_SERVICE_READ_BP_SW_VERSION){
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			p_cm->sdo_service = SDO_SERVICE_IDLE;
		}
		if(p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort && p_cm->sdo_service == SDO_SERVICE_READ_BP_SW_VERSION){
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			p_cm->sdo_service = SDO_SERVICE_IDLE;
		}

		break;
	case CM_ASSIGN_ST_FAIL:
		p_cm->on_slave_assign_fail(p_cm,p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
		break;
	}
}

static CO_Slave* can_master_get_assign_request_slave(const CAN_master *const p_cm) {
	for (uint32_t i = 0; i < p_cm->slave_num; i++) {
		if (p_cm->slaves[i]->con_state == CO_SLAVE_CON_ST_ASSIGNING) {
			return p_cm->slaves[i];
		}
	}
	return NULL;
}

void can_master_update_sn_assign_process(CAN_master *p_cm) {
	switch (p_cm->sn_assign_state) {
	case BMS_MATED_DEV_WRITE_BSS_SN:
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort
				|| p_cm->CO_base.sdo_client.status == CO_SDO_RT_success) {
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
		}
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_idle) {
			p_cm->data_write_bms_od.p_data = &p_cm->hmi_xe_sn[0];
			p_cm->data_write_bms_od.attr = ODA_SDO_RW;
			p_cm->data_write_bms_od.len = 32;
			p_cm->data_write_bms_od.p_ext = NULL;
			CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
					p_cm->slaves[p_cm->slave_id]->node_id, BMS_VEHICLE_SN_INDEX, BMS_MATTED_DEV_SUBINDEX,
					&p_cm->data_write_bms_od,
					SDO_WRITE_OBJ_TIMEOUT_mS);
			p_cm->write++;
			p_cm->sdo_service = SDO_SERVICE_WRITE_SN_XE;
			p_cm->sn_assign_state = BMS_MATED_DEV_CHECK_WRITE_SN_STATE;
		}
		break;
	case BMS_MATED_DEV_CHECK_WRITE_SN_STATE:
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort
				&& p_cm->sdo_service == SDO_SERVICE_WRITE_SN_XE) {
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			p_cm->write_abort++;
			p_cm->sdo_service = SDO_SERVICE_IDLE;
			p_cm->sn_assign_state = BMS_MATED_DEV_WRITE_BSS_SN;
		}
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_success
				&& p_cm->sdo_service == SDO_SERVICE_WRITE_SN_XE) {
			delay_ms++;
			if (delay_ms > WAIT_BP_WRITE_DATA_TO_FLASH) {
				delay_ms = 0;
				p_cm->write_success++;
				p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
				p_cm->serial_number_sobj.attr = ODA_SDO_RW;
				p_cm->serial_number_sobj.p_data =
						p_cm->slaves[p_cm->slave_id]->xe_sn;
				p_cm->serial_number_sobj.len = 32;
				p_cm->serial_number_sobj.p_ext = NULL;
				CO_SDOclient_start_upload(&p_cm->CO_base.sdo_client,
						p_cm->slaves[p_cm->slave_id]->node_id, BMS_VEHICLE_SN_INDEX, BMS_MATTED_DEV_SUBINDEX,
						&p_cm->serial_number_sobj,
						SDO_READ_OBJ_TIMEOUT_mS);
				p_cm->read++;
				p_cm->sdo_service = SDO_SERVICE_READ_SN_XE;
				p_cm->sn_assign_state = BMS_MATED_DEV_CHECK_READ_BSS_SN;
			}
		}
		break;
	case BMS_MATED_DEV_CHECK_READ_BSS_SN:
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_success
				&& p_cm->sdo_service == SDO_SERVICE_READ_SN_XE) {
			p_cm->sdo_service = SDO_SERVICE_IDLE;
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			p_cm->read_success++;
			p_cm->sn_assign_state = BMS_MATED_DEV_DONE;
		}
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort
				&& p_cm->sdo_service == SDO_SERVICE_READ_SN_XE) {
			p_cm->sdo_service = SDO_SERVICE_IDLE;
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			p_cm->read_abort++;
			p_cm->serial_number_sobj.attr = ODA_SDO_RW;
			p_cm->serial_number_sobj.p_data = p_cm->slaves[p_cm->slave_id]->xe_sn;
			p_cm->serial_number_sobj.len = 32;
			p_cm->serial_number_sobj.p_ext = NULL;
			CO_SDOclient_start_upload(&p_cm->CO_base.sdo_client,
					p_cm->slaves[p_cm->slave_id]->node_id, BMS_VEHICLE_SN_INDEX, BMS_MATTED_DEV_SUBINDEX,
					&p_cm->serial_number_sobj,
					SDO_READ_OBJ_TIMEOUT_mS);

			p_cm->sn_assign_state = BMS_MATED_DEV_DONE;
		}
		break;
	case BMS_MATED_DEV_DONE:
		p_cm->sdo_service_xe_sn_done = true;
		p_cm->sn_assign_state = BMS_MATED_DEV_IDLE;
		break;
	default:
		break;
	}
}
void can_master_read_bp_version_software(CAN_master* p_cm, uint8_t cab_id){
	p_cm->serial_number_sobj.attr = ODA_SDO_RW;
	p_cm->serial_number_sobj.p_data = p_cm->slaves[cab_id]->bp_software_version;
	p_cm->serial_number_sobj.len = 32;
	p_cm->serial_number_sobj.p_ext = NULL;
	CO_SDOclient_start_upload(&p_cm->CO_base.sdo_client,
								p_cm->slaves[cab_id]->node_id,
								BMS_VERSION_INDEX,
								BMS_VERSION_SUBINDEX,
								&p_cm->serial_number_sobj,
								SDO_READ_OBJ_TIMEOUT_mS);
	p_cm->sdo_service = SDO_SERVICE_READ_BP_SW_VERSION;
}
static void delay_time_ms(int time_ms){
	int i,j;
	for(i = 0;i<time_ms;i++){
		for(j = 0; j < 40;j++);
	}
}
