/*
 * can_master.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"

uint8_t reassign_attemp_cnt = 0;

CAN_master CAN_master_obj;

static CO_Slave* can_master_get_assign_request_slave(const CAN_master *const p_cm);
static void can_master_process_sdo(CAN_master *p_cm, const uint32_t timestamp);

#if 0
CAN_master* can_master_construct(void) {
	CAN_master *p_cm = (CAN_master*) malloc(sizeof(CAN_master));
	while (p_cm == NULL);
	return p_cm;
}
#endif

void can_master_init(CAN_master *p_cm, CO_Slave **slaves, const uint32_t slave_num, CAN_Hw *p_hw) {
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

static void co_send_sync(CAN_master *p_cm) {
	p_cm->p_hw->can_tx.DLC = 0;
	p_cm->p_hw->can_tx.StdId = CAN_REQ_SYNC_BP_DATA_COBID;
	can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
}

void can_master_process(CAN_master *p_cm, const uint32_t timestamp) {

	if((p_cm->sdo_server.timeout <= timestamp) && (p_cm->sdo_server.timeout != 0) && (p_cm->sdo_server.state == SDO_ST_SENT))
	{
		/*time out*/
		p_cm->sdo_server.state = SDO_ST_FAIL;
		p_cm->sdo_server.timeout = 0;
	}

	if (p_cm->sdo_server.is_new_msg == 1)
	{

		can_master_process_sdo(p_cm, timestamp); // ko can dung vi thu vien moi da ho tro
		p_cm->sdo_server.is_new_msg = 0;
		HAL_CAN_ENABLE_IRQ;
		return;
	}

	/* Send Sync request msg every 2s */ //40s
	if((p_cm->pdo_sync_timestamp <= timestamp) && (p_cm->pdo_sync_timestamp != 0)){
		co_send_sync(p_cm);
		p_cm->pdo_sync_timestamp = timestamp + PDO_READ_BP_DATA_TIME_mS;
	}
}

void can_master_disable_pdo(CAN_master* p_cm){
	p_cm->pdo_sync_timestamp = 0;
	for(uint8_t id = 0; id < p_cm->slave_num; id++){
		p_cm->slaves[id]->inactive_time_ms = 0;
	}
}

static void can_master_process_sdo(CAN_master *p_cm, const uint32_t timestamp) {
	(void)timestamp;

	uint8_t cs = p_cm->p_hw->rx_data[0];
	uint32_t mux;
	uint32_t dlc = p_cm->p_hw->can_tx.DLC;
	uint32_t seg_len = 0;

	switch (cs) {
	case SDO_CS_INIT_READ:
		mux = (((uint32_t) (p_cm->p_hw->rx_data[1])) << 16)
				+ (((uint32_t) (p_cm->p_hw->rx_data[2])) << 8)
				+ (uint32_t) (p_cm->p_hw->rx_data[3]);
		p_cm->sdo_server.object_data_len=(uint32_t)(p_cm->p_hw->rx_data[7])+
				(((uint32_t)(p_cm->p_hw->rx_data[6]))<<8)+
				(((uint32_t)(p_cm->p_hw->rx_data[5]))<<16)+
				(((uint32_t)(p_cm->p_hw->rx_data[4]))<<24);

		if (mux != p_cm->sdo_server.object_mux) {
			p_cm->sdo_server.state = SDO_ST_FAIL;
			p_cm->pdo_sync_timestamp = timestamp + 100;
			return;
		}

		p_cm->sdo_server.buff_offset = 0;
		p_cm->p_hw->tx_data[0] = SDO_CS_SEGMENT_WRITE;
		seg_len = p_cm->sdo_server.object_data_len - p_cm->sdo_server.buff_offset;
		if (seg_len > 7) {
			seg_len = 7;
		}
		p_cm->p_hw->can_tx.DLC = seg_len + 1;
		p_cm->p_hw->tx_data[0] = SDO_CS_SEGMENT_WRITE;
		CO_memcpy(p_cm->p_hw->tx_data + 1, p_cm->sdo_server.tx_data_buff + p_cm->sdo_server.buff_offset, seg_len);
		p_cm->sdo_server.buff_offset += seg_len;
		can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
		break;
	case SDO_CS_SEGMENT_READ:
		p_cm->p_hw->tx_data[0] = SDO_CS_SEGMENT_WRITE;
		seg_len = p_cm->sdo_server.object_data_len - p_cm->sdo_server.buff_offset;
		if (seg_len > 7) {
			seg_len = 7;
		}
		p_cm->p_hw->can_tx.DLC = seg_len + 1;
		p_cm->p_hw->tx_data[0] = SDO_CS_SEGMENT_WRITE;
		CO_memcpy(p_cm->p_hw->tx_data + 1, p_cm->sdo_server.tx_data_buff + p_cm->sdo_server.buff_offset, seg_len);
		p_cm->sdo_server.buff_offset += seg_len;
		can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
		break;
	case SDO_CS_FINISH_READ:
		p_cm->sdo_server.timeout = 0;
		p_cm->sdo_server.state=SDO_ST_SUCCESS;
		p_cm->pdo_sync_timestamp = timestamp + 100;
		break;
	case SDO_CS_INIT_WRITE:
		mux = (((uint32_t) (p_cm->p_hw->rx_data[1])) << 16)
				+ (((uint32_t) (p_cm->p_hw->rx_data[2])) << 8)
				+ (uint32_t) (p_cm->p_hw->rx_data[3]);
		if (mux != p_cm->sdo_server.object_mux) {
			p_cm->sdo_server.state = SDO_ST_FAIL;
			p_cm->pdo_sync_timestamp = timestamp + 100;
			return;
		}

		p_cm->sdo_server.buff_offset = 0;
		p_cm->sdo_server.object_data_len = CO_getUint32(p_cm->p_hw->rx_data + 4);
		p_cm->p_hw->tx_data[0] = SDO_CS_SEGMENT_READ;
		can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
		break;
	case SDO_CS_SEGMENT_WRITE:
		CO_memcpy(p_cm->sdo_server.rx_data_buff + p_cm->sdo_server.buff_offset,	p_cm->p_hw->rx_data + 1, 7);
		p_cm->sdo_server.buff_offset += 7;
		p_cm->p_hw->tx_data[0] = SDO_CS_SEGMENT_READ;
		can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
		break;
	case SDO_CS_FINISH_WRITE:
		CO_memcpy(p_cm->sdo_server.rx_data_buff + p_cm->sdo_server.buff_offset,	p_cm->p_hw->rx_data + 1, dlc - 1);
		p_cm->sdo_server.buff_offset += dlc - 1;
		if (p_cm->sdo_server.buff_offset != p_cm->sdo_server.object_data_len) {
			p_cm->sdo_server.state = SDO_ST_FAIL;
			p_cm->pdo_sync_timestamp = timestamp + 100;
		} else {
			p_cm->sdo_server.timeout = 0;
			p_cm->sdo_server.state = SDO_ST_SUCCESS;
			p_cm->pdo_sync_timestamp = timestamp + 100;
		}
		break;
	case SDO_CS_ABORT:
		p_cm->sdo_server.state = SDO_ST_FAIL;
		p_cm->pdo_sync_timestamp = timestamp + 100;
		break;
	}
}

#if 1
void can_master_start_assign_next_slave(CAN_master *p_cm,const uint32_t timestamp)// not use
{
	p_cm->assigning_slave = can_master_get_assign_request_slave(p_cm);
	if (p_cm->assigning_slave == NULL) {
		p_cm->assign_state = CM_ASSIGN_ST_DONE;
		return;
	}
	for (int i = 0; i < 32; i++) {
		p_cm->assigning_slave->sn[i] = 0;
	}
	p_cm->assign_timeout=timestamp + WAIT_BP_ASSIGN_REQ_TIMEOUT_mS;
	p_cm->assign_state = CM_ASSIGN_ST_WAIT_REQUEST;
	can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id-p_cm->slave_start_node_id);
}
#endif

void can_master_start_assign_slave(CAN_master* p_cm, CO_Slave *slave, const uint32_t timestamp){ //1
	co_slave_set_con_state(slave, CO_SLAVE_CON_ST_ASSIGNING);
//    p_cm->pdo_sync_timestamp = 0;
	p_cm->assigning_slave = slave;
	can_master_slave_select(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
	for(uint8_t i = 0; i < 32; i++){
		p_cm->assigning_slave->sn[i] = 0;
	}
	p_cm->assign_timeout = timestamp + WAIT_BP_ASSIGN_REQ_TIMEOUT_mS;
	p_cm->assign_state = CM_ASSIGN_ST_WAIT_REQUEST;
	can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
}

void cm_start_authorize_slave(CAN_master *p_cm, CO_Slave *slave, uint32_t timestamp)
{
	p_cm->assign_state = CM_ASSIGN_ST_AUTHORIZING;
	can_master_read_slave_sn(p_cm, slave->node_id - p_cm->slave_start_node_id, timestamp);

}
void can_set_read_sn_func_pointer(CAN_master* p_cm,void (*read_serial_number_bp)(void))
{
//	(void)p_cm;
//	CAN_master_obj.read_serial_number_bp = read_serial_number_bp;
	p_cm->read_serial_number_bp = read_serial_number_bp;
}
void can_set_sdo_write_obj_func_pointer(CAN_master* p_cm,void (*sdo_write_object)(void))
{
	p_cm->sdo_write_object = sdo_write_object;
}
void can_master_read_slave_sn(CAN_master *p_cm, uint8_t cab_id, uint32_t timestamp)
{
#if 0
	co_sdo_read_object(p_cm,
			SLAVE_SERIAL_NUMBER_OBJECT_INDEX,
			p_cm->slaves[cab_id]->node_id,
			p_cm->slaves[cab_id]->sn,
			timestamp + SDO_READ_SN_TIMEOUT_mS);
#else
	/*Init SDO data*/
	p_cm->serial_number_sobj.attr 	= ODA_SDO_RW;		//<< [skip] set ODA_SDO_RW
	p_cm->serial_number_sobj.p_data = p_cm->slaves[cab_id]->sn;//<< Address variable receiving data
	p_cm->serial_number_sobj.len 	= 32;				//<< Maximum data size that can be received
	p_cm->serial_number_sobj.p_ext	= NULL;				//<< [option], set NULL if not used
	/*Start upload*/
	CO_SDOclient_start_upload(&p_cm->CO_base.sdo_client,
								p_cm->slaves[cab_id]->node_id,
								BMS_INDEX,
								BMS_SERIAL_NUMBER_OBJECT_SUB_INDEX,
								&p_cm->serial_number_sobj,
								SDO_READ_OBJ_TIMEOUT_mS);
	p_cm->sdo_server.node_id_processing = p_cm->slaves[cab_id]->node_id; //them 4/10/22
//		if(p_cm->read_serial_number_bp != NULL)
//				{
//					p_cm->read_serial_number_bp();
//				}
#endif

}
void can_master_write_bms_object(CAN_master* p_cm, uint8_t cab_id, BMS_OBJ bms_obj, uint32_t timestamp)
{
	switch(bms_obj){
	case BMS_MAINSWITCH:
		/*Init SDO data*/
		p_cm->data_write_bms_od.p_data = &p_cm->bms_mainswitch_state;
		p_cm->data_write_bms_od.attr   = ODA_SDO_RW;
		p_cm->data_write_bms_od.len	   = 1;
		p_cm->data_write_bms_od.p_ext  = NULL;
		/*Start download*/
		CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
									p_cm->slaves[cab_id]->node_id,
									BMS_INDEX,
									BMS_MAINSWITCH_SUB_INDEX,
									&p_cm->data_write_bms_od,
									SDO_WRITE_OBJ_TIMEOUT_mS);
		p_cm->sdo_server.node_id_processing = p_cm->slaves[cab_id]->node_id;	//them 4/10/22

		break;
	case BMS_MATTING:
		/*Init SDO data*/
		p_cm->data_write_bms_od.p_data = &p_cm->bms_matting_state;
		p_cm->data_write_bms_od.attr   = ODA_SDO_RW;
		p_cm->data_write_bms_od.len	   = 1;
		p_cm->data_write_bms_od.p_ext  = NULL;
		/*Start download*/
		CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
									p_cm->slaves[cab_id]->node_id,
									0x2004,
									0x00,
									&p_cm->data_write_bms_od,
									SDO_WRITE_OBJ_TIMEOUT_mS);

		break;
	case BMS_MATED_DEV:
		p_cm->sn_assign_state = BMS_MATED_DEV_WRITE_BSS_SN;
		break;
	default :
		break;
	}
}
#if 1
void co_sdo_read_object(CAN_master *p_cm, const uint32_t mux, const uint32_t node_id,
		uint8_t *rx_buff, const uint32_t timeout)
{
	p_cm->pdo_sync_timestamp = 0;
	p_cm->sdo_server.node_id_processing = node_id;
	p_cm->sdo_server.timeout = timeout;
	p_cm->sdo_server.tx_address = CO_CAN_ID_SDO_SRV + node_id;
	p_cm->sdo_server.rx_address = CO_CAN_ID_SDO_CLI + node_id;
	p_cm->sdo_server.object_mux = mux;
	p_cm->sdo_server.buff_offset = 0;
	p_cm->sdo_server.rx_data_buff = rx_buff;
	p_cm->p_hw->can_tx.StdId = p_cm->sdo_server.tx_address;
	p_cm->p_hw->can_tx.DLC = 4;
	p_cm->p_hw->tx_data[0] = SDO_CS_INIT_READ;
	p_cm->p_hw->tx_data[1] = (uint8_t) ((p_cm->sdo_server.object_mux & 0x00ff0000) >> 16);
	p_cm->p_hw->tx_data[2] = (uint8_t) ((p_cm->sdo_server.object_mux & 0x0000ff00) >> 8);
	p_cm->p_hw->tx_data[3] = (uint8_t) ((p_cm->sdo_server.object_mux & 0x000000ff));
	can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
	p_cm->sdo_server.state = SDO_ST_SENT;
}
#endif

void co_sdo_write_object(CAN_master *p_cm, const uint32_t mux,const uint32_t node_id,
		uint8_t *tx_buff, const uint32_t len, const uint32_t timeout) {
#if 1
	p_cm->pdo_sync_timestamp = 0;
	p_cm->sdo_server.node_id_processing = node_id;
	p_cm->sdo_server.timeout = timeout;
	p_cm->sdo_server.tx_address = CO_CAN_ID_SDO_SRV + node_id;
	p_cm->sdo_server.rx_address = CO_CAN_ID_SDO_CLI + node_id;
	p_cm->sdo_server.object_mux = mux;
	p_cm->sdo_server.buff_offset = 0;
	p_cm->sdo_server.object_data_len = len;
	p_cm->sdo_server.tx_data_buff = tx_buff;
	p_cm->p_hw->can_tx.StdId = p_cm->sdo_server.tx_address;
	p_cm->p_hw->can_tx.DLC = 4;
	p_cm->p_hw->tx_data[0] = SDO_CS_INIT_WRITE;
	p_cm->p_hw->tx_data[1] = (uint8_t) ((p_cm->sdo_server.object_mux & 0x00ff0000) >> 16);
	p_cm->p_hw->tx_data[2] = (uint8_t) ((p_cm->sdo_server.object_mux & 0x0000ff00) >> 8);
	p_cm->p_hw->tx_data[3] = (uint8_t) ((p_cm->sdo_server.object_mux & 0x000000ff));
	can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
	p_cm->sdo_server.state = SDO_ST_SENT;
#endif
}

void can_master_update_id_assign_process(CAN_master *p_cm, const uint32_t timestamp) {
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
		can_master_slave_select(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
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
		can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);///luon on khi giao tiep voi pin
		if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort) //fail
		{
			p_cm->assign_state = CM_ASSIGN_ST_FAIL;
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
		}
		else if (p_cm->CO_base.sdo_client.status == CO_SDO_RT_success)
		{
			co_slave_set_con_state(p_cm->assigning_slave, CO_SLAVE_CON_ST_AUTHORIZING);
			p_cm->on_slave_assign_success(p_cm,	p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
			p_cm->pdo_sync_timestamp = timestamp + 20;
			reassign_attemp_cnt = 0;
//			CO_SDO_get_status(&p_c)
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
		}
		break;
	case CM_ASSIGN_ST_DONE:
		break;
	case CM_ASSIGN_ST_FAIL:
        p_cm->on_slave_assign_fail(p_cm, p_cm->assigning_slave->node_id-p_cm->slave_start_node_id);
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

void can_master_update_sn_assign_process(CAN_master* p_cm){
	uint8_t cab_id ;
	switch(p_cm->sn_assign_state){
	case BMS_MATED_DEV_WRITE_BSS_SN:
		if(p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort || p_cm->CO_base.sdo_client.status == CO_SDO_RT_success){
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			cab_id = p_cm->assigning_slave->node_id - p_cm->slave_start_node_id;
			/*Init SDO data*/
			p_cm->data_write_bms_od.p_data = &p_cm->bss_sn[0];
			p_cm->data_write_bms_od.attr   = ODA_SDO_RW;
			p_cm->data_write_bms_od.len	   = 32;
			p_cm->data_write_bms_od.p_ext  = NULL;
			/*Start download*/
			CO_SDOclient_start_download(&p_cm->CO_base.sdo_client,
										p_cm->slaves[cab_id]->node_id,
										0x2004,
										0x01,
										&p_cm->data_write_bms_od,
										SDO_WRITE_OBJ_TIMEOUT_mS);
			p_cm->sn_assign_state = BMS_MATED_DEV_CHECK_WRITE_SN_STATE;
		}
		break;
	case BMS_MATED_DEV_CHECK_WRITE_SN_STATE:
		if(p_cm->CO_base.sdo_client.status == CO_SDO_RT_abort){

			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
		}
		if(p_cm->CO_base.sdo_client.status == CO_SDO_RT_success){
			uint8_t cab_id = p_cm->assigning_slave->node_id - p_cm->slave_start_node_id;
			p_cm->CO_base.sdo_client.status = CO_SDO_RT_idle;
			/*Init SDO data*/
			if(p_cm->bss_sn[0] == '\0'){
				p_cm->serial_number_sobj.attr 	= ODA_SDO_RW;		//<< [skip] set ODA_SDO_RW
				p_cm->serial_number_sobj.p_data = p_cm->bss_sn;		//<< Address variable receiving data
				p_cm->serial_number_sobj.len 	= 32;				//<< Maximum data size that can be received
				p_cm->serial_number_sobj.p_ext	= NULL;				//<< [option], set NULL if not used
				/*Start upload*/
				CO_SDOclient_start_upload(&p_cm->CO_base.sdo_client,
											p_cm->slaves[cab_id]->node_id,
											BMS_INDEX,
											BMS_SERIAL_NUMBER_OBJECT_SUB_INDEX,
											&p_cm->serial_number_sobj,
											SDO_READ_OBJ_TIMEOUT_mS);
			}else{
				p_cm->sn_assign_state = BMS_MATED_DEV_WRITE_DONE;
			}
		}
		break;
	case BMS_MATED_DEV_WRITE_DONE:
		break;
	default:
		break;
	}
}

