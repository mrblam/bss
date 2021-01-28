/*
 * I2C.c
 *
 *  Created on: Jan 25, 2021
 *      Author: Admin
 */
#include "stm32f10x.h"
#include "I2C.h"
#include "../GPIO_DoorSt/GPIO.h"

#define I2C1_ADRESS 0x40005410;
uint8_t Data_I2C1_Revice[2];

void config_I2C()
{
	//config gpio i2c1
 config_gpio_(GPIOB,GPIO_Pin_6,GPIO_Mode_AF_OD,GPIO_Speed_50MHz);
 config_gpio_(GPIOB,GPIO_Pin_7,GPIO_Mode_AF_OD,GPIO_Speed_50MHz);

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
 I2C_InitTypeDef I2C_STRUCT;
 I2C_STRUCT.I2C_Ack=I2C_Ack_Disable;
 I2C_STRUCT.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
 I2C_STRUCT.I2C_ClockSpeed=100000;
 I2C_STRUCT.I2C_DutyCycle=I2C_DutyCycle_2;
 I2C_STRUCT.I2C_Mode=I2C_Mode_I2C;
 I2C_STRUCT.I2C_OwnAddress1=0;
 I2C_Init(I2C1,&I2C_STRUCT);
 I2C_Cmd(I2C1,ENABLE);
 I2C_ITConfig(I2C1,I2C_IT_RXNE,ENABLE);

 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
 DMA_InitTypeDef DMA_STRUCT;
 DMA_DeInit(DMA1_Channel7);
 DMA_STRUCT.DMA_BufferSize=2;
 DMA_STRUCT.DMA_DIR=DMA_DIR_PeripheralSRC;
 DMA_STRUCT.DMA_M2M=DMA_M2M_Disable;
 DMA_STRUCT.DMA_MemoryBaseAddr=(uint32_t)Data_I2C1_Revice;
 DMA_STRUCT.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
 DMA_STRUCT.DMA_MemoryInc=DMA_MemoryInc_Disable;
 DMA_STRUCT.DMA_Mode=DMA_Mode_Normal;
 DMA_STRUCT.DMA_PeripheralBaseAddr=(uint32_t)I2C1_ADRESS;
 DMA_STRUCT.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
 DMA_STRUCT.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
 DMA_STRUCT.DMA_Priority=DMA_Priority_Medium;
 DMA_Init(DMA1_Channel7,&DMA_STRUCT);

 DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);



  NVIC_InitTypeDef NVIC_STRUCT;
  NVIC_STRUCT.NVIC_IRQChannel=DMA1_Channel7_IRQn ;
  NVIC_STRUCT.NVIC_IRQChannelCmd=ENABLE;
  NVIC_STRUCT.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_STRUCT.NVIC_IRQChannelSubPriority=0;
  NVIC_Init(&NVIC_STRUCT);



}

void READ_I2C(uint8_t slave_adress)
{
	DMA_Cmd(DMA1_Channel7,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel7,2); //SỐ BYTE NHẬN LẠ LÀ 2
	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)){} //CHO ĐÉN KHI KHÔNG CÓ CỜ BÁO BẬN

	I2C_DMALastTransferCmd(I2C1,ENABLE); //BẬT NACK  //RẤT QUAN TRỌNG
    I2C_Cmd(I2C1,ENABLE);

        I2C_GenerateSTART(I2C1,ENABLE);
    	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){}
    	I2C_Send7bitAddress(I2C1,slave_adress,I2C_Direction_Receiver);
    	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){}

    	DMA_Cmd(DMA1_Channel7,ENABLE);
    	I2C_DMACmd(I2C1,ENABLE);


}
uint8_t I2C_DATA=0;
//NHẠN DỮ LIỆU I2C ,ngắt
void DMA1_Channel7_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC7)==SET) //TRANSFER COMPLITION //chuyển dữ liệu hoàn thành
 {
	 DMA_ClearITPendingBit(DMA1_IT_TC7);
	 I2C_DMACmd(I2C1,DISABLE);
	 I2C_GenerateSTOP(I2C1,ENABLE);
	 DMA_Cmd(DMA1_Channel7,DISABLE);

	 //CODE SAU KHI DMA HOÀN THÀNH NHẬN DỮ LIỆU

 }

}


void SEND_BYTE_I2C(uint8_t slave_adress,uint8_t data)
{
	I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1,slave_adress,I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1,data);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1,ENABLE);
}



//WRITE I2C

void SEND_CONTROL_PCF8575(uint8_t slave_adress,uint8_t byte_control_1,uint8_t byte_control_2)
{
	I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1,slave_adress,I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1,byte_control_1);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1,byte_control_2);
     while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1,ENABLE);
}
