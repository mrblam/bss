
#ifndef BOARD_CAN_HARDWARE_H_
#define BOARD_CAN_HARDWARE_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "core_hw.h"

typedef struct CAN_Hw_t CAN_Hw;
struct CAN_Hw_t{
	CAN_HandleTypeDef 	can_module;
	CAN_TxHeaderTypeDef	can_tx;
	CAN_RxHeaderTypeDef	can_rx;
	uint32_t			tx_mailbox;
	uint8_t				tx_data[8];
	uint8_t				rx_data[8];
	void 				(*can_send)(CAN_Hw* p_hw);
	void				(*receive_handle)(CAN_Hw* p_hw);
};

extern CAN_Hw	can_port;

void can_hardware_init(void);
void can_send(CAN_Hw* p_hw, uint8_t* buff);
void can_receive(CAN_Hw* p_hw, uint8_t* buff);

#endif /* BOARD_CAN_HARDWARE_H_ */
