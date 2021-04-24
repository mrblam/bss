/*
 * io_state.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "io_state.h"

IO_State* ios_construct(void){
	IO_State* p_ios = (IO_State*)malloc(sizeof(IO_State));
	while(p_ios == NULL);
	return p_ios;
}

