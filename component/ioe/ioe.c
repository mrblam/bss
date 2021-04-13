/*
 * ioe.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "ioe.h"

static void ioe_set_channel_mask(IOE* p_ioe, uint8_t channel);

IOE* ioe_construct(void){
	IOE* p_ioe = (IOE*)malloc(sizeof(IOE));
	p_ioe->set_channel = ioe_set_channel_mask;

	while(p_ioe == NULL);
	return p_ioe;
}

void ioe_set_channel(IOE* p_ioe, uint8_t channel){
	if(channel < 8){
		p_ioe->p0_data |= 0x01<<channel;
	}
	else{
		p_ioe->p1_data |= 0x01<<(channel - 8);
	}
	ioe_hw_write_reg(p_ioe->hw, p_ioe->hw->address, p_ioe->p0_data);
	ioe_hw_write_reg(p_ioe->hw, p_ioe->hw->address, p_ioe->p1_data);
}

void ioe_clear_channel(IOE* p_ioe, uint8_t channel){

}

