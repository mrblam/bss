/*
 * timer.h
 *
 *  Created on: Oct 8, 2020
 *      Author: quangnd
 */

#ifndef SERVICE_TIMER_TIMER_H_
#define SERVICE_TIMER_TIMER_H_
#include "stdint.h"
#include "timer_hal.h"

typedef struct Timer
{
    Timer_Hw* p_hw;
} Timer;

static inline void timer_set_interval_ms(Timer* p_tmr,const uint32_t interval){

	p_tmr->p_hw->timeout=interval;
}

static inline void timer_reset_ticker(Timer* p_tmr){
	p_tmr->p_hw->ticker=0;
}

static inline void timer_free(Timer* p_tmr){
        p_tmr->p_hw->is_active=0;
}

void TimerInit(Timer*);
uint8_t TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, uint32_t);
void TimerCountdown(Timer*, uint32_t);
int32_t TimerLeftMS(Timer*);

#endif /* SERVICE_TIMER_TIMER_H_ */
