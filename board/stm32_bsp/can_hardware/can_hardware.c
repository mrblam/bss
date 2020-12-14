#include "can_hardware.h"
#include "string.h"
#include "interrupt_hw.h"
#include "stm32f10x.h"

CAN_Hw can1;
static void can_set_baudrate_impl(CAN_Hw* p_hw,uint32_t baudrate) ;
static void can_send_impl(CAN_Hw* p_hw,CanTxMsg* p_msg);

static void can_hardware_init_clk(void);
static void can_hardware_init_gpio(void);
static void can_hardware_init_module(void);
static void can_hardware_init_nvic(void);

void can_hardware_init(void) {
	can_hardware_init_clk();
	can_hardware_init_gpio();
	can_hardware_init_module();
	can_hardware_init_nvic();
	can1.send=can_send_impl;
	can1.set_baudrate=can_set_baudrate_impl;
}

static void can_hardware_init_clk(void) {
}

static void can_hardware_init_gpio(void) {
}

static void can_hardware_init_module(void) {

}

static void can_hardware_init_nvic(void) {

}

uint8_t can_tx_available(CAN_Hw* p_hw){

  /* Select one empty transmit mailbox */
  if ((p_hw->can_module->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
  {
	  return 1;
  }

  if ((p_hw->can_module->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
  {
	  return 1;
  }

  if ((p_hw->can_module->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
  {
	  return 1;
  }
  return 0;
}
static void can_send_impl(CAN_Hw* p_hw,CanTxMsg* p_msg){
	while(!can_tx_available(p_hw)){

	}
	CAN_Transmit(p_hw->can_module,p_msg);
}

static void can_set_baudrate_impl(CAN_Hw* p_hw,uint32_t baudrate) {
	p_hw->baudrate=baudrate;
}

void CEC_CAN_IRQHandler(void) {
}


