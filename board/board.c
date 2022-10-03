#include "board.h"

void board_init(void){
	HAL_Init();
	core_hw_init();
	uart_hw_init();
	max485_hw_init();
	can_hardware_init();
	cabinet_hw_init();
	timer_hw_init();
	//adc_hw_init();
	master_hw_init();
}
