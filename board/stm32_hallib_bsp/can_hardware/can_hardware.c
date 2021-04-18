#include "can_hardware.h"
#include "string.h"
#include "interrupt_hw.h"

CAN_HandleTypeDef 	hcan;
CAN_TxHeaderTypeDef	TxHeader;
CAN_RxHeaderTypeDef	RxHeader;

static void can_hardware_init_clk(void);
static void can_hardware_init_module(void);
static void can_hardware_filter_init(void);
static void can_hardware_init_nvic(void);
static void can_hardware_start(void);
static void can_hardware_init_tx(void);

void can_hardware_init(void) {
	can_hardware_init_clk();
	can_hardware_init_module();
	can_hardware_init_nvic();
	can_hardware_filter_init();
	can_hardware_start();
	can_hardware_init_tx();
}

static void can_hardware_init_clk(void) {
	__HAL_RCC_CAN1_CLK_ENABLE();
}

static void can_hardware_filter_init(void){
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
}

static void can_hardware_init_module(void) {
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
	if(HAL_CAN_Init(&hcan) != HAL_OK){
		Error_Handler();
	}
}

static void can_hardware_init_nvic(void) {

}

static void can_hardware_start(void){
	if (HAL_CAN_Start(&hcan) != HAL_OK){
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK){
		Error_Handler();
	}
}

static void can_hardware_init_tx(void){
	TxHeader.StdId = RSDO_ID + MASTER_ID;
	TxHeader.DLC = 8;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.TransmitGlobalTime = DISABLE;
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}
