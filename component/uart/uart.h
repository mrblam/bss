/*
 * uart.h
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_UART_UART_H_
#define COMPONENT_UART_UART_H_

#include "stdint.h"
#include "stdlib.h"

#include "../bp_data/bp_data.h"
#include "debug_com_port_hal.h"
#include "cabinet_cell.h"
#include "app_config.h"
#include "string_util.h"

typedef struct BP_UART_Data_t BP_UART_Data;
typedef struct CAB_UART_Data_t CAB_UART_Data;
typedef struct BSS_UART_Data_t BSS_UART_Data;

struct BP_UART_Data_t{
    char* (*serializer)(BP* p_bp, char* buff);
};

struct CAB_UART_Data_t{
	char* (*serializer)(Cabinet* p_cc, char* buff);
};

struct BSS_UART_Data_t{
	uint8_t state;
	uint8_t tilt_ss_state;
	uint8_t fan_state;
	uint8_t charger_state;
	int32_t temp;
	char* (*serializer)(BSS_UART_Data* p_bss, char* buff);
};



#endif /* COMPONENT_UART_UART_H_ */
