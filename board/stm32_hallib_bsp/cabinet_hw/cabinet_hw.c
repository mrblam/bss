/*
 * cabinet_hw.c
 *
 *  Created on: Apr 22, 2021
 *      Author: nguyenquang
 */
#include "cabinet_hw.h"
#include "gpio_hw.h"

static void cabinet_io_scan_timer_init(void);

uint32_t door_state;
void cabinet_hw_init(void){
	cabinet_io_scan_timer_init();
	gpio_hw_init();
}

static void cabinet_io_scan_timer_init(void){

}

