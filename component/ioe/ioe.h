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
	uint8_t channel;
	uint8_t p0_data;
	uint8_t p1_data;
	void (*set_channel)(IOE* p_ioe, uint8_t channel);
	void (*clear_channel)(IOE* p_ioe, uint8_t channel);
};

IOE* ioe_construct(void);


#endif /* COMPONENT_IOE_IOE_H_ */
