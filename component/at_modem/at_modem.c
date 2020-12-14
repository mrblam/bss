/*
 * at_modem.c
 *
 *  Created on: Oct 7, 2020
 *      Author: quangnd
 */
#include "at_modem.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"

void at_modem_init(AT_Modem* p_modem){
	p_modem->tx_cmd[0]='A';
	p_modem->tx_cmd[1]='T';
	p_modem->tx_cmd[2]='+';
	p_modem->state=AT_MODEM_ST_INACTIVE;
}

static uint8_t str_contain(const char* dst,const char* str){

        const char* match= str;
        const char* source=dst;
	while(*match){
		if((*source)!= (*match)) return 0;
		if(*source=='\0') return 0;
		source++;
		match++;
	}
	return 1;
}

int32_t at_modem_search_response_string(AT_Modem* p_modem,const char* str){

	int32_t len=strlen(str);
	if(p_modem->rx_index < len) return-1;
	for(int i=0;i<p_modem->rx_index;i++){
		if(str_contain((char*)(p_modem->rx_buffer+i),str)) return i;
	}
	return -1;
}

/* send command and wait for specific response */
int32_t at_modem_send_cmd(AT_Modem* p_modem,const char* res,const uint32_t timeout){

        int32_t rc=-1;
	Timer timer;
	TimerInit(&timer);
	at_modem_flush_rx_buffer(p_modem);
	TimerCountdownMS(&timer,timeout);
	p_modem->send((char*)p_modem->tx_cmd);
	while ((at_modem_search_response_string(p_modem,res) < 0) && !TimerIsExpired(&timer)) {
	        at_modem_read_all(p_modem);
	}
	if(TimerIsExpired(&timer)){
	        rc=-1;
	}else{
	        rc=0;
	}
	timer_free(&timer);
	return rc;
}
