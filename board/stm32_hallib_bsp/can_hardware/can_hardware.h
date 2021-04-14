
#ifndef BOARD_CAN_HARDWARE_H_
#define BOARD_CAN_HARDWARE_H_
#include "stdint.h"

typedef struct CAN_Hw_t CAN_Hw;

struct CAN_Hw_t{
};


extern CAN_Hw can1;

void can_hardware_init(void);

#endif /* BOARD_CAN_HARDWARE_H_ */
