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
#include "app_config.h"

typedef enum BSS_STATE{
	BSS_ST_ACTIVE = 0,
	BSS_ST_MAINTAIN,
	BSS_ST_FAIL,
	BSS_ST_INIT
} BSS_STATE;

typedef struct Charger_t Charger;
struct Charger_t{
	uint8_t			assigned_cab_num;
	Cabinet* 		assigned_cabs[MAX_ASSIGNED_CAB_NUM];
	Cabinet*		charging_cabin;
	Switch			input_power;
};

typedef struct BSS_Data_t BSS_Data;
struct BSS_Data_t{
	BSS_STATE 		state;
	uint8_t 		cab_num;
	uint8_t			charger_num;
	IO_State	 	tilt_ss;
	Charger*		ac_chargers;
	Switch			bss_fans[2];
	Switch 			bss_lamps[4];
	uint8_t 		bss_temps[2];
	Cabinet*        cabs;
	void			(*data_serialize)(BSS_Data* p_bss_data, char* buff);
	uint8_t 		is_changed;
};

void bss_init(BSS_Data* p_bss);
void bss_update_cabinets_state(BSS_Data* p_bss);
void bss_set_state(BSS_Data* p_bss, BSS_STATE state);
void bss_update_charge_state(BSS_Data* p_bss);
Cabinet* bss_get_cab_need_charge(BSS_Data* p_bss, uint8_t charger_id);

static inline void bss_data_serialize(BSS_Data* p_bss_data, char* buff){
	p_bss_data->data_serialize(p_bss_data, buff);
}

#endif /* COMPONENT_BSS_DATA_BSS_DATA_H_ */
