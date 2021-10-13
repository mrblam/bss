/*
 * peripheral_init.h
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#ifndef APP_PERIPHERAL_INIT_PERIPHERAL_INIT_H_
#define APP_PERIPHERAL_INIT_PERIPHERAL_INIT_H_

#include "board.h"
#include "cabinet_app.h"
#include "rs485_master.h"

extern Cabinet_App		selex_bss_app;
extern uint32_t 	sys_timestamp;
extern RS485_Master 	rs485m;

void peripheral_init(Cabinet_App* p_ca);

#endif /* APP_PERIPHERAL_INIT_PERIPHERAL_INIT_H_ */
