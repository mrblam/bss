#include "board.h"
#include "core_hal.h"
#include "can_hal.h"
#include "adc_hw_hal.h"
#include "debug_io_hw_hal.h"
#include "ioe_hal.h"

void board_init(void){
        core_hw_init();
    	SysTick_Config(SystemCoreClock/1000);
        debug_io_hw_init();
	//debug_com_hw_init();
	can_hardware_init();
	ioe_hw_init();
}
