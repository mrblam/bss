/*
 * ioe_hw.c
 *
 *  Created on: Dec 19, 2020
 *      Author: quangnd
 */
#include "ioe_hw.h"

IOE_Hw ioe_module;

static void ioe_write_impl(IOE_Hw* p_hw, const uint8_t addr,uint8_t* data);
static void i2c_init(void);

void ioe_hw_init(void){
	i2c_init();
	ioe_module.ioe_write = ioe_write_impl;
}

static void i2c_init(void){
	ioe_module.i2c_com.Instance = I2C1;
	ioe_module.i2c_com.Init.ClockSpeed = 100000;
	ioe_module.i2c_com.Init.DutyCycle = I2C_DUTYCYCLE_2;
	ioe_module.i2c_com.Init.OwnAddress1 = 0;
	ioe_module.i2c_com.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	ioe_module.i2c_com.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	ioe_module.i2c_com.Init.OwnAddress2 = 0;
	ioe_module.i2c_com.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	ioe_module.i2c_com.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&ioe_module.i2c_com) != HAL_OK){
		Error_Handler();
	}
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  }
}

static void ioe_write_impl(IOE_Hw* p_hw, const uint8_t addr, uint8_t* data){
	HAL_I2C_Master_Transmit(&p_hw->i2c_com, addr, data, 2, 500);
}

