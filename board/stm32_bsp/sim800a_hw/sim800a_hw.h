/*
 * hc05_hw.h
 *
 *  Created on: Oct 5, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_SIM800A_HW_H_
#define BOARD_STM32_BSP_SIM800A_HW_H_
#include "stdint.h"

typedef void (*Sim800a_Receive_Handle)(const char* buff,const uint16_t len);

void sim800a_hw_init(void);
void sim800a_send(const char ch);
void sim800a_sends(const char* s);
void sim800a_send_len(const char* s,const int32_t len);
int32_t sim800a_read_all(char* buff);
void sim800a_clear_rx_buffer(void);
#endif /* BOARD_STM32_BSP_HC05_HW_HC05_HW_H_ */
