/*
 * I2C.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Admin
 */

#ifndef I2C_I2C_H_
#define I2C_I2C_H_

#include "stm32f10x.h"
extern uint8_t Data_I2C1_Revice[2];

void config_I2C();
void SEND_CONTROL_PCF8575(uint8_t slave_adress,uint8_t byte_control_1,uint8_t byte_control_2);
void SEND_BYTE_I2C(uint8_t slave_adress,uint8_t data);
void READ_I2C(uint8_t slave_adress);
#endif /* I2C_I2C_H_ */
