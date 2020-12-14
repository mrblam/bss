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
#include "sim800a_hal.h"
#include "at_modem.h"

#define TEST_CMD				"TEST_CMD"

static AT_Modem modem;
static void sim800_on_receive(const char ch);

static void app_init(void){
	global_interrupt_disable();
	board_init();
	at_modem_init(&modem);
	modem.send=sim800a_sends;
	sim800a_set_receive_handle(sim800_on_receive);
	global_interrupt_enable();
}

int main(void){
	app_init();
	static uint32_t cmd_id=0;
	char cmd_buffer[20];
	while(1){
		long_to_string(cmd_id,cmd_buffer);
		at_modem_set_cmd(&modem,cmd_buffer);
		at_modem_send_cmd(&modem);
		cmd_id++;
		while(modem.cmd_state !=MODEM_ST_RESPONSE_FINISH){
			__NOP();
		}

	}
	return 0;
}

static void sim800_on_receive(const char ch){

	at_modem_receive_handle(&modem,ch);
}
void HAL_STATE_MACHINE_UPDATE_TICK(void){

}
