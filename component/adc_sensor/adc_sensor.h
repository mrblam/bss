/*
 * adc_sensor.h
 *
 *  Created on: Aug 22, 2020
 *      Author: quangnd
 */

#ifndef COMPONENT_ADC_SENSOR_ADC_SENSOR_H_
#define COMPONENT_ADC_SENSOR_ADC_SENSOR_H_

#include "stdint.h"
#include "adc_hw_hal.h"
#include "app_config.h"

typedef struct ADC_Sensor_t ADC_Sensor;

struct ADC_Sensor_t{
	ADC_hw* hw;
	int32_t result;
	int32_t gain;
	void (*update_result)(ADC_Sensor* p_ss);
};

void adc_sensor_init(ADC_Sensor* p_ss);

static inline int32_t adc_sensor_get_result(const ADC_Sensor* const p_ss){
	return p_ss->result;
}

static inline void adc_sensor_update_result(ADC_Sensor* p_ss){
	p_ss->update_result(p_ss);
}

#endif /* COMPONENT_ADC_SENSOR_ADC_SENSOR_H_ */
