/*
 * uart_hw_hal.h
 *
 *  Created on: Nov 20, 2020
 *      Author: quangnd
 */

#ifndef BOARD_STM32_BSP_UART_HW_H_
#define BOARD_STM32_BSP_UART_HW_H_

#include "stdint.h"
#include "string.h"
#include "stdio.h"

#define UART_RX_BUFF_SIZE                    64
typedef struct Uart_Hw_t Uart_Hw;

struct Uart_Hw_t{
        uint32_t rx_index;
        char buff[UART_RX_BUFF_SIZE];
        void (*send)(const char*);
        void (*sends)(const char*);
        void (*send_len)(const char*,const uint16_t len);
        void (*receive)(const char* buff,const uint16_t len);
};

static inline void uart_clear_rx_buffer(Uart_Hw* p_hw){
        p_hw->rx_index=0;
        p_hw->buff[0]=0;
}

static inline int32_t uart_read_all(Uart_Hw* p_hw,char* buff){
        memcpy(buff,&p_hw->buff[0],p_hw->rx_index);
        p_hw->rx_index=0;
        return p_hw->rx_index;
}
#endif /* BOARD_STM32_BSP_UART_HW_H_ */
