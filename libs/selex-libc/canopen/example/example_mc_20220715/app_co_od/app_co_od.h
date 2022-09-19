/*
 * app_co_od.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Dakaka
 *
 *      Only edit in USER CODE 1
 */

#ifndef APP_APP_CANOPEN_APP_CO_OD_H_
#define APP_APP_CANOPEN_APP_CO_OD_H_

#include "app_co_comm_od.h"
#include "app_co_manu_od.h"

extern CO_OD *p_co_od;

/* USER CODE BEGIN 1 */

/* Unit test*/
typedef struct
{
	uint8_t test1;
	uint16_t test2;
	uint32_t test3;
}test_t;
extern test_t od_data_test_mapping_tpdo;
extern uint8_t od_data_test_expe_sdo[2];
extern uint8_t od_data_test_normal_sdo[100];

/* USER CODE END 1 */

#endif /* APP_APP_CANOPEN_APP_CO_OD_H_ */
