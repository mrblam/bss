#ifndef _BOARD_H_
#define _BOARD_H_
#include "interrupt_hal.h"
#include "compiler_optimize.h"
#include "stdint.h"
#include "hw_delay_hal.h"
#include "stm32f1xx_hal.h"
#include "uart_hw_hal.h"
#include "core_hal.h"

void board_init(void) WEAK;
void global_interrupt_enable(void) WEAK;
void global_interrupt_disable(void) WEAK;


#endif
