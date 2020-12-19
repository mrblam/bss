/*
 * cell_storage.h
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */

#ifndef COMPONENT_STORAGE_CELL_STORAGE_CELL_H_
#define COMPONENT_STORAGE_CELL_STORAGE_CELL_H_

#include "stdint.h"
#include "ntc.h"
#include "switch.h"

typedef struct Storage_Cell_t Storage_Cell;

struct Storage_Cell_t{
        uint8_t id;
        int32_t temp;
        NTC* temp_ss;
        Switch* ven_fan_sw;
        Switch* door_solenoid;
        uint8_t door_state;
        uint8_t bp_state;
};

Storage_Cell* storage_cell_create(void);
void storage_cell_fan_on(Storage_Cell* p_cell);
void storage_cell_fan_off(Storage_Cell* p_cell);
void storage_cell_update_temp(Storage_Cell* p_cell);
int32_t storage_cell_get_temp(Storage_Cell* p_cell);

#endif /* COMPONENT_STORAGE_CELL_STORAGE_CELL_H_ */
