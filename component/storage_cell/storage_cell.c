/*
 * cell_storage.c
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */
#include "storage_cell.h"
#include "stdlib.h"
#include "stdio.h"

Storage_Cell* storage_cell_create(void){

        Storage_Cell* p_cell=(Storage_Cell*)malloc(sizeof(Storage_Cell));
        while(p_cell==NULL){};

        p_cell->temp_ss=(NTC*) malloc(sizeof(NTC));
        while(p_cell->temp_ss==NULL){};

        p_cell->ven_fan_sw=(Switch*)malloc(sizeof(Switch));
        while(p_cell->ven_fan_sw==NULL){};
        return p_cell;
}

void storage_cell_fan_on(Storage_Cell* p_cell){

}

void storage_cell_fan_off(Storage_Cell* p_cell){

}

void storage_cell_update_temp(Storage_Cell* p_cell){

        ntc_update_temp(p_cell->temp_ss);
}

int32_t storage_cell_get_temp(Storage_Cell* p_cell){
        return p_cell->temp;
}





