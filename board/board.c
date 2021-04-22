#include "board.h"
#include "core_hal.h"
#include "gpio_hal.h"

void board_init(void){
	HAL_Init();
	core_hw_init();
	uart_hw_init();
	timer_hw_init();
	gpio_hw_init();
	can_hardware_init();
	cabinet_hw_init();
}
