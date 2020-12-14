/*
 * sim800a_hal.h
 *
 *  Created on: Oct 5, 2020
 *      Author: quangnd
 */

#ifndef BOARD_SIM800A_HAL_H_
#define BOARD_SIM800A_HAL_H_
#include "compiler_optimize.h"
#include "sim800a_hw.h"
#include "stdint.h"

void sim800a_hw_init(void) WEAK;
void sim800a_send(const char ch) WEAK;
void sim800a_sends(const char* s) WEAK;
void sim800a_send_len(const char* s,const int32_t len) WEAK;
void sim800a_set_receive_handle(Sim800a_Receive_Handle handle) WEAK;
int32_t sim800a_read_all(char* buff)WEAK;
void sim800a_clear_rx_buffer(void) WEAK;

#endif /* BOARD_SIM800A_HAL_H_ */
