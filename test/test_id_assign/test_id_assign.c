/*
 * test_id_assign.c
 *
 *  Created on: Apr 17, 2021
 *      Author: KhanhDinh
 */
#include "board.h"
#include "stm32f1xx_hal.h"


int main(void){
	__disable_irq();
	board_init();
	can_init();
}

void can_init(void){
	can_config();
	can_gpio_config();
}

static void can_config(void){
	__HAL_RCC_CAN1_CLK_ENABLE();
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 1;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_7TQ;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoRetransmission = DISABLE;
  	hcan.Init.AutoWakeUp = DISABLE;
  	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
  	HAL_CAN_Init(hcan)
}

static void can_gpio_config(void){

}
