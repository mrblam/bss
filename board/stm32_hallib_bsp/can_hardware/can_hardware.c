#include "can_hardware.h"
#include "string.h"
#include "interrupt_hw.h"

CAN_Hw can1;

static void can_hardware_init_clk(void);
static void can_hardware_init_gpio(void);
static void can_hardware_init_module(void);
static void can_hardware_init_nvic(void);

void can_hardware_init(void) {
	can_hardware_init_clk();
	can_hardware_init_gpio();
	can_hardware_init_module();
	can_hardware_init_nvic();
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

}
