
#ifndef BOARD_CAN_HARDWARE_H_
#define BOARD_CAN_HARDWARE_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "core.h"
#include "app_config.h"

typedef struct CAN_Hw_t CAN_Hw;

extern CAN_HandleTypeDef 	hcan;
extern CAN_TxHeaderTypeDef	TxHeader;
extern CAN_RxHeaderTypeDef	RxHeader;

void can_hardware_init(void);

#endif /* BOARD_CAN_HARDWARE_H_ */
