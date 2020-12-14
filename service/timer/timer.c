/*
 * timer.c
 *
 *  Created on: Oct 8, 2020
 *      Author: quangnd
 */

#include "timer.h"
#include "stdint.h"
#include "timer_hal.h"

void TimerInit(Timer* p_tmr){

        p_tmr->p_hw=timer_hw_create();
        timer_reset_ticker(p_tmr);
        p_tmr->p_hw->timeout=0;
        p_tmr->p_hw->is_active=1;
}

uint8_t TimerIsExpired(Timer* p_tmr){
	if(p_tmr->p_hw->timeout<=p_tmr->p_hw->ticker) return 1;
	return 0;
}

void TimerCountdownMS(Timer* p_tmr, uint32_t ms){
	
	timer_set_interval_ms(p_tmr,ms);
	timer_reset_ticker(p_tmr);
}

void TimerCountdown(Timer* p_tmr, uint32_t val){
	TimerCountdownMS(p_tmr,1000*val);
}

int32_t TimerLeftMS(Timer* p_tmr){

	int32_t left=(int32_t)p_tmr->p_hw->timeout-(int32_t)p_tmr->p_hw->ticker;
	if(left<0) return 0;
	return left;
}





