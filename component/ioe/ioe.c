/*
 * ioe.c
 *
 *  Created on: Apr 12, 2021
 *      Author: KhanhDinh
 */

#include "ioe.h"

IOE solenoid;
IOE cell_fan;

void ioe_init(void){
	uint32_t cnt;
	solenoid.address = 0x40;
	cell_fan.address = 0x41;
	ioe_clear_all(&solenoid);
	while(cnt < 100000) cnt++;
	cnt = 0;
	ioe_clear_all(&solenoid);
	while(cnt < 100000) cnt++;
	cnt = 0;
	ioe_clear_all(&solenoid);
	while(cnt < 100000) cnt++;
	cnt = 0;
	ioe_clear_all(&solenoid);
	while(cnt < 100000) cnt++;
	cnt = 0;
	ioe_clear_all(&solenoid);
	while(cnt < 100000) cnt++;
	cnt = 0;
	ioe_clear_all(&cell_fan);
}

void ioe_set_channel(IOE* p_ioe, uint8_t channel){
	if(channel < 8){
		p_ioe->port_data[0] |= 0x01<<channel;
	}
	else{
		p_ioe->port_data[1] |= 0x01<<(channel - 8);
	}
	ioe_hw_write(p_ioe->hw, p_ioe->address, p_ioe->port_data);
}

void ioe_clear_channel(IOE* p_ioe, uint8_t channel){
	if(channel < 8){
		p_ioe->port_data[0]  &= ~(0x01 << channel);
	}
	else{
		p_ioe->port_data[1]  &= ~(0x01 << (channel - 8));
	}
	ioe_hw_write(p_ioe->hw, p_ioe->address, p_ioe->port_data);
}

void ioe_set_all(IOE* p_ioe){
	p_ioe->port_data[0] = p_ioe->port_data[1] = 0xff;
	ioe_hw_write(p_ioe->hw, p_ioe->address, p_ioe->port_data);
}

void ioe_clear_all(IOE* p_ioe){
	p_ioe->port_data[0] = p_ioe->port_data[1] = 0x00;
	ioe_hw_write(p_ioe->hw, p_ioe->address, p_ioe->port_data);
}

