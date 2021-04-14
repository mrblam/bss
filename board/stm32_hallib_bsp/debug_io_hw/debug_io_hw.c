/*
 * debug_io_hw.c
 *
 *  Created on: Oct 9, 2020
 *      Author: quangnd
 */

#include "debug_io_hw.h"

static void debug_io_rcc_init(void);
static void debug_io_gpio_init(void);

void debug_io_hw_init(void){
	debug_io_rcc_init();
	debug_io_gpio_init();
}

static void debug_io_rcc_init(void){
}

static void debug_io_gpio_init(void){
}
