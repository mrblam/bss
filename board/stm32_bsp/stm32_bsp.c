#include "stm32f0xx.h"
#include "can_hardware.h"
#include "stm32f0xx_it.h"
#include "debug_com_port_hardware.h"
#include "core_hw.h"
#include "sim800a_hw.h"
#include "debug_io_hw.h"
#include "timer_hw.h"

void board_init(void){
        core_hw_init();
        debug_io_hw_init();
	//debug_com_hw_init();
	timer_hw_init();
	can_hardware_init();
	sim800a_hw_init();
}

void global_interrupt_enable(void){
	__enable_irq();
}
void global_interrupt_disable(void){
	__disable_irq();
}

