/*
 * test_solenoid.c
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#include "ioe.h"
#include "peripheral_init.h"

IOE solenoid;

int main(void){
	ioe_hw_init();
	solenoid.address = 0x40;
	ioe_clear_all(&solenoid);
	while(1){

	}
}


