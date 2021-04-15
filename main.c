/*
 * main.c
 *
 *  Created on: Aug 18, 2020
 *      Author: quangnd
 */
#include "board.h"
#include "delay.h"
#include "can_hal.h"
#include "string_util.h"
#include "app_config.h"

static volatile uint32_t sys_timestamp=0;
static volatile uint32_t sync_counter=0;

static void app_init(void){
	global_interrupt_disable();
	board_init();
	global_interrupt_enable();
}

int main(void){
	app_init();
	while(1){
	}
	return 0;
}

void HAL_STATE_MACHINE_UPDATE_TICK(void) {

        sys_timestamp+=APP_STATE_MACHINE_UPDATE_TICK_mS;
        sync_counter++;
}
