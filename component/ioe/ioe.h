/*
 * ioe.h
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_IOE_IOE_H_
#define COMPONENT_IOE_IOE_H_

#include "stdlib.h"
#include "stdint.h"
#include "ioe_hal.h"

typedef struct IOE_t IOE;
struct IOE_t{
	IOE_Hw* hw;
	uint8_t address;
	uint8_t channel;
	uint8_t port_data[2];
};

extern IOE solenoid;
extern IOE cell_fan;


void ioe_init(void);
void ioe_set_channel(IOE* p_ioe, uint8_t channel);
void ioe_clear_channel(IOE* p_ioe, uint8_t channel);
void ioe_set_all(IOE* p_ioe);
void ioe_clear_all(IOE* p_ioe);

#endif /* COMPONENT_IOE_IOE_H_ */
