#include "board.h"
#include "core_hal.h"
#include "can_hal.h"
#include "adc_hw_hal.h"
#include "debug_io_hw_hal.h"
#include "uart_hw_hal.h"

void board_init(void){
	HAL_Init();
	core_hw_init();
	uart_hw_init();
}
