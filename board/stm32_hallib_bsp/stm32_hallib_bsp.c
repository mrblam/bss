#include "can_hardware.h"
#include "core_hw.h"
#include "debug_io_hw.h"
#include "ioe_hw.h"
#include "uart_hw/uart_hw.h"

void global_interrupt_enable(void){
	__enable_irq();
}
void global_interrupt_disable(void){
	__disable_irq();
}

