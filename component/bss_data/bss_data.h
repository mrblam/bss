/*
 * bss_data.h
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BSS_DATA_BSS_DATA_H_
#define COMPONENT_BSS_DATA_BSS_DATA_H_

#include "stdint.h"
#include "stdlib.h"
#include "string_util.h"
#include "cabinet_cell.h"

typedef enum BSS_STATE{
	BSS_ST_STANDBY,
	BSS_ST_ACTIVE,
	BSS_ST_MAINTAIN,
	BSS_ST_FAIL
} BSS_STATE;

typedef enum TILT_SS_STATE{
	TILT_SS_INACTIVE,
	TILT_SS_ACTIVE,
	TILT_SS_FAIL
} TILT_SS_STATE;

typedef enum CHARGER_STATE{
	CHARGER_ST_INACTIVE,
	CHARGER_ST_ACTIVE,
	CHARGER_ST_FAIL
} CHARGER_STATE;

typedef enum FAN_STATE{
	FAN_ST_INACTIVE,
	FAN_ST_ACTIVE,
	FAN_ST_FAIL,
} FAN_STATE;

typedef enum LAMP_STATE{
	LAMP_ST_INACTIVE,
	LAMP_ST_ACTIVE,
	LAMP_ST_FAIL,
} LAMP_STATE;

typedef struct BSS_Data_t BSS_Data;
struct BSS_Data_t{
	char			serial_number[32];
	BSS_STATE 		state;
	uint8_t 		cab_num;
	TILT_SS_STATE 	tilt_ss_state;
	CHARGER_STATE 	charger_state;
	FAN_STATE		fan_state;
	LAMP_STATE 		lamp_state;
	int32_t 		temp;
	Cabinet*           cabs;
	void			(*data_serialize)(BSS_Data* p_bss_data, char* buff);
};

BSS_Data* bss_data_construct(void);

static inline void bss_data_serialize(BSS_Data* p_bss_data, char* buff){
	p_bss_data->data_serialize(p_bss_data, buff);
}

void bss_update_cabinets_state(BSS_Data* p_bss);
void bss_update_io_state(BSS_Data* p_bss);

#endif /* COMPONENT_BSS_DATA_BSS_DATA_H_ */
