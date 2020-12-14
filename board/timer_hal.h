#ifndef _BOARD_TIMER_HAL_H_
#define _BOARD_TIMER_HAL_H_

#include "timer_hw.h"
#include "compiler_optimize.h"

void timer_hw_init(void) WEAK;
Timer_Hw* timer_hw_create(void) WEAK;
#endif
