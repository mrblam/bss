#include "board.h"
#include "core_hal.h"

void board_init(void){
	HAL_Init();
	core_hw_init();
	uart_hw_init();
	timer_hw_init();
	gpio_init();
	can_hardware_init();
	adc_hw_init();
}
