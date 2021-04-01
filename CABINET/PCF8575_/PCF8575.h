/*
 * PCF8575.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Admin
 */

#ifndef PCF8575__PCF8575_H_
#define PCF8575__PCF8575_H_

#include "stm32f10x.h"

	uint8_t FAN1=1;
	uint8_t FAN2=2;
	uint8_t FAN3=3;
	uint8_t FAN4=4;
	uint8_t FAN5=5;
	uint8_t FAN6=6;
	uint8_t FAN7=7;
	uint8_t FAN8=8;
	uint8_t FAN9=9;
	uint8_t FAN10=10;
	uint8_t FAN11=11;
	uint8_t FAN12=12;
	uint8_t FAN13=13;
	uint8_t FAN14=14;
	uint8_t FAN15=15;
	uint8_t FAN16=16;

void ON_OFF_FAN(uint8_t slave_adress,uint8_t FANx,uint8_t ON_OFF);

#endif /* PCF8575__PCF8575_H_ */
