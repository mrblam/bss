/*
 * test_AT_Modem.c
 *
 *  Created on: Oct 7, 2020
 *      Author: quangnd
 */

#include "board.h"
#include "delay.h"
#include "can_hal.h"
#include "string_util.h"
#include "debug_io_hw_hal.h"
#include "timer.h"

static void app_init(void){
	global_interrupt_disable();
	board_init();
	global_interrupt_enable();
}

static Timer test_timer;

static void timer_start(const uint32_t interval){

	TimerInit(&test_timer);
	TimerCountdownMS(&test_timer,interval);
}

int main(void){
	app_init();
	while(1){
			IO1_SET_HIGH;
			timer_start(1000);
			while(TimerIsExpired(&test_timer)==0){

			};
			IO1_SET_LOW;
			timer_start(500);
			while(TimerIsExpired(&test_timer)==0){

			};
	}
	return 0;
}

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}
