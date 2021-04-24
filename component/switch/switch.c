/*
 * switch.c
 *
 *  Created on: Aug 21, 2020
 *      Author: quangnd
 */

#include "switch.h"

Switch* sw_construct(void){
	Switch* p_sw = (Switch*)malloc(sizeof(Switch));
	while(p_sw == NULL);
	return p_sw;
}


