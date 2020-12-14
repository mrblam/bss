/*
 * can_hal.h
 *
 *  Created on: Aug 20, 2020
 *      Author: quangnd
 */

#ifndef BOARD_CAN_HAL_H_
#define BOARD_CAN_HAL_H_
#include "can_hardware.h"
#include "compiler_optimize.h"

void can_hardware_init(void) WEAK;
static inline void can_set_receive_handle(CAN_Hw* p_hw,CAN_Receive_Handle handle){

        p_hw->receive_handle=handle;
}

static inline void can_send(CAN_Hw* p_hw,CanTxMsg* p_msg){
	p_hw->send(p_hw,p_msg);
}

static inline void can_set_baudrate(CAN_Hw* p_hw,const uint32_t baud){
	p_hw->set_baudrate(p_hw,baud);
}

#endif /* BOARD_CAN_HAL_H_ */
