/*
 * gpio_hw.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#include "gpio_hw.h"

static void gpio_led_pin_init(void);
static void gpio_fan_init(void);
static void gpio_door_st_pin_init(void);

void gpio_hw_init(void){
	gpio_led_pin_init();
	gpio_fan_init();
	gpio_door_st_pin_init();
}

static void gpio_led_pin_init(void){

}

static void gpio_fan_init(void){

}

static void gpio_door_st_pin_init(void){

}

/* -------------------------------------------------------------------------- */



