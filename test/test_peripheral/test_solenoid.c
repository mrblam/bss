/*
 * test_solenoid.c
 *
 *  Created on: Apr 24, 2021
 *      Author: KhanhDinh
 */

#include "ioe.h"
#include "cabinet_app.h"
#include "peripheral_init.h"

IOE solenoid;

int main(void){
	ioe_hw_init();
	solenoid.address = 0x40;
	ioe_clear_all(&solenoid);
	for(uint8_t i = CAB1; i <= CAB15; i++){
		ioe_set_channel(&solenoid,i);
	}

	while(1){

	}
}


