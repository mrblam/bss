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

typedef struct BSS_Data_t BSS_Data;
struct BSS_Data_t{
	int32_t	serial_number;
	uint8_t state;
	uint8_t cab_num;
	uint8_t active_cab_num;
	uint8_t tilt_ss_state;
	uint8_t charger_state;
	uint8_t fan_state;
	uint8_t lamp_state;
	int32_t temp;
	void	(*data_serialize)(BSS_Data* p_bss_data, char* buff);
};

BSS_Data* bss_data_construct(void);

static inline void bss_data_serialize(BSS_Data* p_bss_data, char* buff){
	p_bss_data->data_serialize(p_bss_data, buff);
}

#endif /* COMPONENT_BSS_DATA_BSS_DATA_H_ */
