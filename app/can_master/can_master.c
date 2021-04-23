/*
 * can_master.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "can_master.h"

CAN_master* can_master_construct(void){
	CAN_master* p_cm = (CAN_master*)malloc(sizeof(CAN_master));
	while(p_cm == NULL);
	return p_cm;
}

void can_master_init(CAN_master* p_cm,CO_Slave** slaves,const uint32_t slave_num, CAN_Hw* p_hw){
	p_cm->slaves=slaves;
	p_cm->p_hw=p_hw;
	p_cm->sdo_server.state=SDO_ST_IDLE;
	p_cm->slave_num=slave_num;
}

CO_Slave* can_master_get_assign_request_slave(const CAN_master* const p_cm){
	for(uint32_t i=0;i<p_cm->slave_num;i++){
		if(p_cm->slaves[i]->con_state==CO_SLAVE_CON_ST_ASSIGNING){
			return p_cm->slaves[i];
		}
	}
	return NULL;
}

void can_master_process(CAN_master* p_cm,const uint32_t timestamp){

	/* if assign request message */
	if(p_cm->p_hw->can_rx.StdId==TSDO_ID){
		if(p_cm->assign_state==ASSIGN_ST_DONE){
		        can_master_start_assign_next_slave_id(p_cm);
		}
		else if(p_cm->assign_state==ASSIGN_ST_WAIT_CONFIRM){
		        /* slave confirm assign id success*/
		        if(p_cm->p_hw->rx_data[0]==p_cm->assigning_slave->node_id){
		                /* finish assign for current slave and move to next slave */
		                p_cm->assigning_slave->con_state=CO_SLAVE_CON_ST_AUTHORIZING;
		                can_master_start_assign_next_slave_id(p_cm);
		        }
		}
	}


	switch(p_cm->assign_state){
	case ASSIGN_ST_START:
                can_master_slave_select(p_cm, p_cm->assigning_slave->node_id);
                p_cm->p_hw->can_tx.StdId=RSDO_ID;
                p_cm->p_hw->can_tx.DLC=1;
                p_cm->p_hw->tx_data[0]=p_cm->assigning_slave->node_id;
                can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
                p_cm->assign_state=ASSIGN_ST_WAIT_CONFIRM;
                p_cm->assign_timeout=timestamp+100;
	        break;
	case ASSIGN_ST_WAIT_CONFIRM:
	        if(p_cm->assign_timeout>timestamp){
	                p_cm->assigning_slave->con_state =
                                      CO_SLAVE_CON_ST_DISCONNECT;
	                can_master_start_assign_next_slave_id(p_cm);
                }
	        break;
	case ASSIGN_ST_DONE:
	        break;
	}

	if(p_cm->p_hw->can_rx.StdId==p_cm->sdo_server.rx_address){
		can_master_process_sdo(p_cm, timestamp);
	}
}

void can_master_process_sdo(CAN_master* p_cm,const uint32_t timestamp){

	if(p_cm->sdo_server.timeout>=timestamp){
		p_cm->sdo_server.state=SDO_ST_FAIL;
		return;
	}

	uint8_t cs=p_cm->p_hw->rx_data[0];
	uint32_t mux;
	uint32_t dlc=p_cm->p_hw->can_tx.DLC;

	switch(cs){
	case SDO_CS_INIT_READ:
	        break;
	case SDO_CS_SEGMENT_READ:
	        break;
	case SDO_CS_FINISH_READ:
	        break;
	case SDO_CS_INIT_WRITE:
	        mux=CO_getUint32(p_cm->p_hw->tx_data);
	        mux=(mux & 0x00ffffff);
	        if(mux!=p_cm->sdo_server.object_mux){
	                p_cm->sdo_server.state=SDO_ST_FAIL;
	        }

                CO_memcpy(p_cm->sdo_server.rx_data_buff, p_cm->p_hw->rx_data+4, dlc);
                if(dlc<=4){
                        p_cm->sdo_server.state=SDO_ST_SUCCESS;
                }else{
                        p_cm->p_hw->tx_data[0]=SDO_CS_SEGMENT_READ;
                        can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
                        p_cm->sdo_server.buff_offset+=dlc;
                }

	        break;
	case SDO_CS_SEGMENT_WRITE:
	        CO_memcpy(p_cm->sdo_server.rx_data_buff+p_cm->sdo_server.buff_offset,
	                        p_cm->p_hw->tx_data+1,7);
	        p_cm->sdo_server.buff_offset+=7;
                p_cm->p_hw->tx_data[0]=SDO_CS_SEGMENT_READ;
                can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
	        break;
	case SDO_CS_FINISH_WRITE:
	        CO_memcpy(p_cm->sdo_server.rx_data_buff+p_cm->sdo_server.buff_offset,
	                        p_cm->p_hw->tx_data+1,dlc);
                        p_cm->sdo_server.state=SDO_ST_SUCCESS;
	        break;
	case SDO_CS_ABORT:
	        break;
	}
}

void can_master_start_assign_next_slave_id(CAN_master *p_cm) {
        p_cm->assigning_slave = can_master_get_assign_request_slave(p_cm);
        if (p_cm == NULL) {
                p_cm->assign_state = ASSIGN_ST_DONE;
                return;
        }

        p_cm->assign_state = ASSIGN_ST_START;
        p_cm->p_hw->can_tx.StdId = RSDO_ID;
        can_send(p_cm->p_hw, p_cm->p_hw->tx_data);
}

void can_master_read_slave_sn(CAN_master* p_cm, uint8_t cab_id){
        p_cm->sdo_server.tx_address=0x580+p_cm->slaves[cab_id]->node_id;
        p_cm->sdo_server.object_mux=0x333;
        p_cm->sdo_server.buff_offset=0;
        p_cm->sdo_server.rx_data_buff=(uint8_t*) p_cm->slaves[cab_id]->sn;
        p_cm->p_hw->tx_data[0]=SDO_CS_INIT_READ;
        p_cm->p_hw->can_tx.StdId=p_cm->sdo_server.tx_address;
        p_cm->p_hw->tx_data[1]= (uint8_t)((p_cm->sdo_server.object_mux& 0x00ffffff)>>16);
        p_cm->p_hw->tx_data[2]= (uint8_t)((p_cm->sdo_server.object_mux& 0x0000ffff)>>8);
        p_cm->p_hw->tx_data[3]= (uint8_t)(p_cm->sdo_server.object_mux& 0x000000ff);
}
