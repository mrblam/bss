#ifndef TIMER_HW_H_
#define TIMER_HW_H_
#include "stm32f0xx_tim.h"

void timer_hw_init(void);
typedef struct Timer_Hw_t Timer_Hw;
struct Timer_Hw_t* timer_hw_create(void);
struct Timer_Hw_t{
        uint32_t ticker;
        uint8_t is_active;
        uint32_t timeout;
};

#endif
