/*
 * bp_data.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "bp_data.h"

static void bp_data_serialize_impl(BP* p_bp, char* buff);
static BP bp_arr[20] ;
BP* bp_construct(uint8_t id){
	BP* p_bp = &bp_arr[id];
	p_bp->state = BP_ST_IDLE;
	p_bp->charge_sw_state = 0;
	p_bp->vol = 0;
	p_bp->cur = 0;
	p_bp->soc = 0;
	p_bp->soh = 0;
	p_bp->cycle = 0;
	p_bp->pos = id;
	p_bp->status = BP_STT_OK;
	p_bp->is_changed = 0;
	p_bp->is_data_available = 0;
	for(uint8_t i = 0; i < 16; i++) p_bp->cell_vol[i] = 0;
	for(uint8_t i = 0; i < 8; i++) p_bp->temp[i] = 0;
	for(uint8_t i = 0; i < 32; i++) p_bp->base.sn[i] = 0;
	for(uint8_t i = 0; i < 32; i++) p_bp->base.xe_sn[i] = 0;
	for(uint8_t i = 0; i < 32; i++) p_bp->base.bp_software_version[i] = 0;
	p_bp->data_serialize = bp_data_serialize_impl;
	return p_bp;
}

void bp_reset_data(BP* p_bp){
	bp_set_con_state(p_bp, CO_SLAVE_CON_ST_DISCONNECT);
	p_bp->state = BP_ST_INIT;
//	p_bp->base.inactive_time_ms = 0;
	p_bp->is_data_available = 0;
	p_bp->vol = 0;
}

static void bp_data_serialize_impl(BP* p_bp, char* buff){
	*buff++ = ':';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'B';
	*buff++ = ',';
	buff += long_to_string(p_bp->pos, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	if (p_bp->base.sn[0] == '\0') {
		*buff++ = '0';
	} else {
		for (uint8_t i = 0; *(p_bp->base.sn + i) != '\0'; i++) {

			*buff++ = *(p_bp->base.sn + strlen((const char*) p_bp->base.sn) - 1 - i);
		}
	}
	*buff++ = ',';
	buff += long_to_string(p_bp->state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bp->status, buff);
	*buff++ = ',';
	buff += long_to_string(p_bp->vol, buff);
	*buff++ = ',';
	if (p_bp->cur < 0) {
		*buff++ = '-';
		buff += long_to_string((-1) * p_bp->cur, buff);
	} else
		buff += long_to_string(p_bp->cur, buff);
	*buff++ = ',';
	buff += long_to_string(p_bp->cycle, buff);
	*buff++ = ',';
	buff += long_to_string(p_bp->soc, buff);
	*buff++ = ',';
	buff += long_to_string(p_bp->soh, buff);
	*buff++ = ',';
	if (p_bp->base.xe_sn[0] == '\0') {
		*buff++ = '0';
	} else {
		for (uint8_t i = 0; *(p_bp->base.xe_sn + i) != '\0'; i++) {

			*buff++ = *(p_bp->base.xe_sn + i);
		}
	}
	*buff++ = ',';
	if (p_bp->base.bp_software_version[0] == '\0') {
		*buff++ = '0';
	} else {
		buff += long_to_string(p_bp->base.bp_software_version[2],buff);
		*buff++ = '.';
		buff += long_to_string(p_bp->base.bp_software_version[1],buff);
		*buff++ = '.';
		buff += long_to_string(p_bp->base.bp_software_version[0],buff);
	}
	*buff++ = ',';
	*buff++ = '[';
	for (uint8_t i = 0; i < 16; i++) {
		buff += long_to_string(p_bp->cell_vol[i], buff);
		*buff++ = ',';
	}
	*--buff = ']';
	buff++;
	*buff++ = ',';
	*buff++ = '[';
	for (uint8_t i = 0; i < 8; i++) {
		buff += long_to_string(p_bp->temp[i], buff);
		*buff++ = ',';
	}
	*--buff = ']';
	buff++;
	*buff++ = '*';
	*buff++ = '\n';
	*buff++ = '\0';
}
