/*
 * ntc.c
 *
 *  Created on: Sep 30, 2020
 *      Author: quangnd
 */

#include "ntc.h"

static int16_t ntc_get_temp_from_impedance(const uint16_t impedance,const uint16_t* const p_lut,
		const uint16_t size,const int16_t min){

	int16_t id=0;
    while((impedance < p_lut[id]) &&(id<size)){
   	 id++;
    }
	return(id+min);
}

void ntc_update_temp(NTC* p_ntc){

	ntc_update_impedance(p_ntc);

	p_ntc->temp=ntc_get_temp_from_impedance(p_ntc->impedance,p_ntc->lut,p_ntc->lut_size,
			p_ntc->min_temp);
}


