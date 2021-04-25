
#ifndef BOARD_CAN_HARDWARE_H_
#define BOARD_CAN_HARDWARE_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "core.h"
#include "app_config.h"

typedef struct CAN_hw_t CAN_hw;
struct CAN_hw_t{
	CAN_HandleTypeDef 	can_module;
	CAN_TxHeaderTypeDef	can_tx;
	CAN_RxHeaderTypeDef	can_rx;
	uint32_t			tx_mailbox;
	uint8_t				tx_data[8];
	uint8_t				rx_data[8];
	void 				(*can_send)(CAN_hw* p_hw);
	void				(*receive_handle)(CAN_hw* p_hw);
};

extern CAN_hw	can_port;

void can_hardware_init(void);

#endif /* BOARD_CAN_HARDWARE_H_ */
