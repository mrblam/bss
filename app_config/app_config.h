/*
 * app_config.h
 *
 *  Created on: Oct 27, 2020
 *      Author: quangnd
 */

#ifndef APP_CONFIG_APP_CONFIG_H_
#define APP_CONFIG_APP_CONFIG_H_
#include "core_hal.h"

#define APP_STATE_MACHINE_UPDATE_TICK_mS                (1000/HAL_SYSTICK_FREQ_Hz)
#define BP_INACTIVE_TIMEOUT_mS                                  5000

#endif /* APP_CONFIG_APP_CONFIG_H_ */
