/*
 * mux.h
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_MUX_MUX_H_
#define COMPONENT_MUX_MUX_H_

#include "stdint.h"
#include "stdlib.h"
#include "gpio_hal.h"

typedef struct MUX_t MUX;
typedef void (*mux_act)(MUX* p_mux);
struct MUX_t{
	uint8_t channel;
	mux_act switch_channel;
};

extern MUX mux_ntc;

void mux_switch_channel(MUX* p_mux, uint8_t channel);

#endif /* COMPONENT_MUX_MUX_H_ */
