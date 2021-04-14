#ifndef DEBUG_PORT_H_
#define DEBUG_PORT_H_
#include "stm32f10x.h"

typedef struct UART_HW_t UART_HW;
struct UART_HW_t{
	USART_TypeDef* uart_module;
};

extern UART_HW power_sync_port;
extern UART_HW	hmi_sync_port;

void uart_hw_init(void);
void uart_sends(UART_HW* p_hw,const uint8_t* s);

/* Configuration Pins for Power System Sync Data */
#define POWER_SYNC_PORT        				USART1
#define POWER_SYNC_PORT_CLK            		RCC_APB2Periph_USART1

#define POWER_SYNC_PORT_TX_PIN             	GPIO_Pin_9
#define POWER_SYNC_PORT_TX_GPIO_PORT        GPIOA
#define POWER_SYNC_PORT_TX_GPIO_CLK         RCC_AHBPeriph_GPIOA
#define POWER_SYNC_PORT_TX_SOURCE           GPIO_PinSource9
#define POWER_SYNC_PORT_TX_AF               GPIO_AF_1

#define POWER_SYNC_PORT_RX_PIN              GPIO_Pin_10
#define POWER_SYNC_PORT_RX_GPIO_PORT        GPIOA
#define POWER_SYNC_PORT_RX_GPIO_CLK         RCC_AHBPeriph_GPIOA
#define POWER_SYNC_PORT_RX_SOURCE           GPIO_PinSource10
#define POWER_SYNC_PORT_RX_AF               GPIO_AF_1

/* Configuration Pins for HMI Sync Data */
#define HMI_SYNC_PORT						USART2
#define	HMI_SYNC_PORT_CLK					RCC_APB1Periph_USART2

#define HMI_SYNC_PORT_TX_PIN				GPIO_Pin_2
#define HMI_SYNC_PORT_TX_GPIO_PORT			GPIOA
#define HMI_SYNC_PORT_TX_GPIO_CLK			RCC_AHBPeriph_GPIOA
#define HMI_SYNC_PORT_TX_SOURCE				GPIO_PinSource2
#define HMI_SYNC_PORT_TX_AF					GPIO_AF_1

#define HMI_SYNC_PORT_RX_PIN				GPIO_Pin_3
#define HMI_SYNC_PORT_RX_GPIO_PORT			GPIOA
#define HMI_SYNC_PORT_RX_GPIO_CLK			RCC_AHBPeriph_GPIOA
#define	HMI_SYNC_PORT_RX_SOURCE				GPIO_PinSource3
#define	HMI_SYNC_PORT_RX_AF					GPIO_AF_1

#define HMI_SYNC_PORT_IRQn                  USART2_IRQn
#define HMI_SYNC_PORT_HANLDE				USART2_IRQHandler
#define HMI_SYNC_PORT_IRQn_Priority		  	3

//typedef void (*Uart_Receive_Handle)();

void debug_com_hw_init(void);
//void debug_port_set_receive_handle(Uart_Receive_Handle handle);
//void USART_Write_String(char *a);
void uart_sends(UART_HW* p_hw,const uint8_t* s);

#endif
