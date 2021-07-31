/*
 * bp_data.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "bp_data.h"

static void bp_data_serialize_impl(BP* p_bp, char* buff);

BP* bp_construct(void){
	BP* p_bp = (BP*)malloc(sizeof(BP));
	while(p_bp == NULL);

	p_bp->state = BP_ST_IDLE;
	p_bp->charge_sw_state=0;

	p_bp->data_serialize = bp_data_serialize_impl;
	return p_bp;
}

void bp_reset_data(BP* p_bp){
	bp_set_con_state(p_bp, CO_SLAVE_CON_ST_DISCONNECT);
	p_bp->state = BP_ST_INIT;
	p_bp->base.inactive_time_ms = 0;
	p_bp->is_data_available = 0;
	p_bp->vol = 0;
}

static void bp_data_serialize_impl(BP* p_bp, char* buff){
	*buff++=':';
	*buff++='R';
    *buff++=',';
	*buff++='B';
    *buff++=',';
    buff+=long_to_string(p_bp->pos, buff);
    *buff++=',';
    *buff++='A';
    *buff++=',';
    if(p_bp->base.sn[0] == '\0'){
    	*buff++='0';
    }
    else{
        for(uint8_t i = 0; *(p_bp->base.sn + i) != '\0'; i++){
        	*buff++= *(p_bp->base.sn + i);
        }
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
