/*
 * bp_data.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "bp_data.h"

static void bp_data_serialize_impl(BP* p_bp, char* buff);

static const char* default_serial_number="Selex_Battery";

BP* bp_construct(void){
	BP* p_bp = (BP*)malloc(sizeof(BP));
	while(p_bp == NULL);

	p_bp->state = BP_ST_IDLE;
	p_bp->charge_sw_state=0;

    char* sn=(char*)default_serial_number;
    char* sn_buff=(char*)p_bp->base.sn;
    while(*sn){
            *sn_buff++=*sn++;
    }
    *sn_buff++='\0';

	p_bp->data_serialize = bp_data_serialize_impl;
	return p_bp;
}

static void bp_data_serialize_impl(BP* p_bp, char* buff){
	*buff++=':';
	*buff++='R';
    *buff++=',';
	*buff++='B';
    *buff++=',';
    buff+=long_to_string(p_bp->pos, buff);
    *buff++=',';
    for(uint8_t i = 0; *(p_bp->base.sn + i) != '\0'; i++){
    	*buff++= *(p_bp->base.sn + i);
    }
    *buff++=',';
	buff+=long_to_string(p_bp->state,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->status,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->vol,buff);
    *buff++=',';
    if(p_bp->cur < 0){
    	*buff++='-';
    	buff+=long_to_string((-1)*p_bp->cur,buff);
    }
    else buff+=long_to_string(p_bp->cur,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->cycle,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->soc,buff);
    *buff++=',';
	buff+=long_to_string(p_bp->soh,buff);
    *buff++=',';
    *buff++='[';
    for(uint8_t i = 0; i < 16; i++){
    	buff+=long_to_string(p_bp->cell_vol[i], buff);
    	*buff++=',';
    }
    *--buff=']';
    buff++;
    *buff++=',';
    *buff++='[';
    for(uint8_t i = 0; i < 8; i++){
    	buff+=long_to_string(p_bp->temp[i], buff);
    	*buff++=',';
    }
    *--buff=']';
    buff++;
    *buff++='*';
    *buff++='\n';
    *buff++='\0';
}
