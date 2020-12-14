/*
 * timer_hw.h
 *
 *  Created on: Oct 11, 2020
 *      Author: nguyenquang
 */

#ifndef TEST_TEST_AT_COMMAND_TIMER_HW_H_
#define TEST_TEST_AT_COMMAND_TIMER_HW_H_
#include "stdint.h"

typedef struct Timer_Hw_t Timer_Hw;
struct Timer_Hw_t* timer_hw_create(void);
struct Timer_Hw_t{
        uint32_t ticker;
        uint8_t is_enabled;
        uint8_t is_active;
        uint32_t timeout;
};

#endif /* TEST_TEST_AT_COMMAND_TIMER_HW_H_ */
