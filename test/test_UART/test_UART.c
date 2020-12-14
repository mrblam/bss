/*
 * test_UART.c
 *
 *  Created on: Sep 9, 2020
 *      Author: son19
 */

/*		Description
 * 	Gui du lieu thong qua UART voi chu ki 250 1 lan
 * 	Du lieu duoc gui la 0x0A
 *	hw_delay_ms(250) va sua doi trong hw_delay_ms : hw_delay_us(80 -> 8)
 */


#include "delay_hw.h"
#include "core_hal.h"
#include "sim800a_hal.h"

const char* test_str="AT\n";
static void sim800a_on_receive(const char ch);

static void test_setup(void){
	core_hw_init();
	sim800a_hw_init();
	sim800a_set_receive_handle(sim800a_on_receive);
}

int main(void)
{
	hw_delay_ms(500);
	test_setup();
	while(1){
		sim800a_sends(test_str);
		hw_delay_ms(1000);
	}
	return 0;
}

void SysTick_Handler(void){

}
static uint8_t buffer[512];
static uint16_t index=0;

static void sim800a_on_receive(const char ch){

	buffer[index++]=ch;
	if(index==512){
		index=0;
	}
}
