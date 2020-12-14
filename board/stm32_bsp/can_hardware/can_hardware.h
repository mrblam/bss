
#ifndef BOARD_CAN_HARDWARE_H_
#define BOARD_CAN_HARDWARE_H_
#include "stm32f10x.h"

#define CAN_DEV_CLK				RCC_APB1Periph_CAN1

#define CAN_TX_PIN				GPIO_Pin_9
#define CAN_TX_PORT				GPIOB
#define CAN_TX_PORT_CLK			        RCC_APB2Periph_GPIOB
#define CAN_TX_PINSOURCE		        GPIO_PinSource9

#define CAN_RX_PIN				GPIO_Pin_8
#define CAN_RX_PORT				GPIOB
#define CAN_RX_PORT_CLK			        RCC_AHBPeriph_GPIOB
#define CAN_RX_PINSOURCE		        GPIO_PinSource8

#define CAN_PRE_DEFAULT			        6

typedef struct CAN_Hw_t CAN_Hw;
typedef void (*CAN_Receive_Handle)(CAN_Hw* p_hw);
typedef void (*CAN_Set_Baudrate)(CAN_Hw* p_hw,const uint32_t baud);
typedef void (*CAN_Send)(CAN_Hw* p_hw,CanTxMsg* p_msg);

struct CAN_Hw_t{
	CAN_TypeDef* can_module;
	uint8_t node_id;
	uint32_t baudrate;
	CAN_Receive_Handle receive_handle;
	CAN_Set_Baudrate set_baudrate;
	CAN_Send send;
	CanRxMsg rx_msg;
};


extern CAN_Hw can1;

void can_hardware_init(void);

#endif /* BOARD_CAN_HARDWARE_H_ */
