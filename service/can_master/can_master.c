/*
 * can_master.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"

static CO_Slave* can_master_get_assign_request_slave(const CAN_master *const p_cm);
static void can_master_process_sdo(CAN_master *p_cm, const uint32_t timestamp);

CAN_master* can_master_construct(void) {
	CAN_master *p_cm = (CAN_master*) malloc(sizeof(CAN_master));
	while (p_cm == NULL);
	return p_cm;
}

void can_master_init(CAN_master *p_cm, CO_Slave **slaves, const uint32_t slave_num, CAN_Hw *p_hw) {
	p_cm->node_id_scan_cobid = CAN_NODE_ID_ASSIGN_COBID;
	p_cm->slaves = slaves;
	p_cm->p_hw = p_hw;
	p_cm->sdo_server.state = SDO_ST_IDLE;
	p_cm->sdo_server.is_new_msg = 0;
	p_cm->slave_num = slave_num;
}

static void co_send_sync(CAN_master *p_cm) {
	p_cm->p_hw->can_tx.DLC = 0;
	p_cm->p_hw->can_tx.StdId = 0x80;
	can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
}

void can_master_process(CAN_master *p_cm, const uint32_t timestamp) {

	if (p_cm->sdo_server.is_new_msg == 1) {
		can_master_process_sdo(p_cm, timestamp);
		p_cm->sdo_server.is_new_msg = 0;
	}

	if((p_cm->pdo_sync_timestamp == timestamp) && (p_cm->pdo_sync_timestamp != 0)){
		co_send_sync(p_cm);
		p_cm->pdo_sync_timestamp += 2000;
	}
}

static void can_master_process_sdo(CAN_master *p_cm, const uint32_t timestamp) {

	if (p_cm->sdo_server.timeout >= timestamp) {
		p_cm->sdo_server.state = SDO_ST_FAIL;
		return;
	}

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
		p_cm->sdo_server.state=SDO_ST_SUCCESS;
		break;
	case SDO_CS_INIT_WRITE:
		mux = (((uint32_t) (p_cm->p_hw->rx_data[1])) << 16)
				+ (((uint32_t) (p_cm->p_hw->rx_data[2])) << 8)
				+ (uint32_t) (p_cm->p_hw->rx_data[3]);
		if (mux != p_cm->sdo_server.object_mux) {
			p_cm->sdo_server.state = SDO_ST_FAIL;
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
		} else {
			p_cm->sdo_server.state = SDO_ST_SUCCESS;
		}
		break;
	case SDO_CS_ABORT:
		p_cm->sdo_server.state = SDO_ST_FAIL;
		break;
	}
}

void can_master_start_assign_next_slave(CAN_master *p_cm,const uint32_t timestamp) {
	p_cm->assigning_slave = can_master_get_assign_request_slave(p_cm);
	if (p_cm->assigning_slave == NULL) {
		p_cm->assign_state = CM_ASSIGN_ST_DONE;
		return;
	}
	for (int i = 0; i < 32; i++) {
		p_cm->assigning_slave->sn[i] = 0;
	}
	p_cm->assign_timeout=timestamp+2000;
	p_cm->assign_state = CM_ASSIGN_ST_WAIT_REQUEST;
	can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id-p_cm->slave_start_node_id);
	/*
	p_cm->p_hw->can_tx.StdId = p_cm->node_id_scan_cobid;
	p_cm->p_hw->can_tx.DLC = 0;
	can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
	*/
}

void can_master_start_assign_slave(CAN_master* p_cm, CO_Slave *slave, const uint32_t timestamp){
    co_slave_set_con_state(slave, CO_SLAVE_CON_ST_ASSIGNING);
    p_cm->pdo_sync_timestamp = 0;
	p_cm->assigning_slave = slave;
	for(uint8_t i = 0; i < 32; i++){
		p_cm->assigning_slave->sn[i] = 0;
	}
	p_cm->assign_timeout = timestamp + 2000;
	p_cm->assign_state = CM_ASSIGN_ST_WAIT_REQUEST;
	can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
}

void cm_start_authorize_slave(CAN_master *p_cm, CO_Slave *slave) {
    //co_slave_set_con_state(slave, CO_SLAVE_CON_ST_AUTHORIZING);
	p_cm->assign_state = CM_ASSIGN_ST_AUTHORIZING;
	can_master_read_slave_sn(p_cm, slave->node_id - p_cm->slave_start_node_id);
}

void can_master_read_slave_sn(CAN_master *p_cm, uint8_t cab_id) {
	/*
	 p_cm->sdo_server.tx_address = 0x580 + p_cm->slaves[cab_id]->node_id;
	 p_cm->sdo_server.rx_address = 0x600 + p_cm->slaves[cab_id]->node_id;
	 p_cm->sdo_server.object_mux = SLAVE_SERIAL_NUMBER_OBJECT_INDEX;
	 p_cm->sdo_server.buff_offset = 0;
	 p_cm->sdo_server.rx_data_buff = (uint8_t*) p_cm->slaves[cab_id]->sn;
	 p_cm->p_hw->can_tx.StdId = p_cm->sdo_server.tx_address;
	 p_cm->p_hw->can_tx.DLC=4;
	 p_cm->p_hw->tx_data[0] = SDO_CS_INIT_READ;
	 p_cm->p_hw->tx_data[1] =(uint8_t)((p_cm->sdo_server.object_mux & 0x00ff0000)>>16);
	 p_cm->p_hw->tx_data[2] =(uint8_t)((p_cm->sdo_server.object_mux&0x0000ff00)>>8);
	 p_cm->p_hw->tx_data[3] =(uint8_t)((p_cm->sdo_server.object_mux& 0x000000ff));
	 can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
	 p_cm->sdo_server.state=SDO_ST_SENT;
	 */
	co_sdo_read_object(p_cm, SLAVE_SERIAL_NUMBER_OBJECT_INDEX, p_cm->slaves[cab_id]->node_id, p_cm->slaves[cab_id]->sn, 0);
}

void co_sdo_read_object(CAN_master *p_cm, const uint32_t mux, const uint32_t node_id,
		uint8_t *rx_buff, const uint32_t timeout) {
	(void)timeout;
	p_cm->sdo_server.tx_address = 0x580 + node_id;
	p_cm->sdo_server.rx_address = 0x600 + node_id;
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

void co_sdo_write_object(CAN_master *p_cm, const uint32_t mux,const uint32_t node_id,
		uint8_t *tx_buff, const uint32_t len, const uint32_t timeout) {
	(void)timeout;
	p_cm->sdo_server.tx_address = 0x580 + node_id;
	p_cm->sdo_server.rx_address = 0x600 + node_id;
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
		p_cm->assign_timeout = timestamp + 2000;
		break;
	case CM_ASSIGN_ST_AUTHORIZING:
		can_master_slave_deselect(p_cm, p_cm->assigning_slave->node_id-p_cm->slave_start_node_id);
		if (p_cm->sdo_server.state == SDO_ST_FAIL){
			p_cm->assign_state = CM_ASSIGN_ST_FAIL;
			p_cm->sdo_server.state = SDO_ST_IDLE;
		}
		else if (p_cm->sdo_server.state == SDO_ST_SUCCESS){
			co_slave_set_con_state(p_cm->assigning_slave, CO_SLAVE_CON_ST_AUTHORIZING);
			p_cm->on_slave_assign_success(p_cm,	p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
			//co_slave_set_con_state(p_cm->assigning_slave, CO_SLAVE_CON_ST_CONNECTED);
			//can_master_slave_deselect(p_cm,	p_cm->assigning_slave->node_id - p_cm->slave_start_node_id);
			p_cm->sdo_server.state = SDO_ST_IDLE;
		}
		break;
	case CM_ASSIGN_ST_DONE:
		break;
	case CM_ASSIGN_ST_FAIL:
        p_cm->on_slave_assign_fail(p_cm, p_cm->assigning_slave->node_id-p_cm->slave_start_node_id);
        co_slave_set_con_state(p_cm->assigning_slave, CO_SLAVE_CON_ST_DISCONNECT);
		p_cm->assign_state = CM_ASSIGN_ST_DONE;
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



