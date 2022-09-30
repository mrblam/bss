#include "uart_hw.h"

UART_hw hmi_com;
UART_hw rs485_com;
UART_hw debug_com;

static void uart_hmi_hw_init(void);
static void uart_rs485_hw_init(void);
static void uart_debug_hw_init(void);

void uart_hw_init(void){
	uart_hmi_hw_init();
	uart_rs485_hw_init();
	uart_debug_hw_init();
}

static void uart_hmi_hw_init(void){
	hmi_com.uart_module.Instance = HMI_PORT_COM;
	hmi_com.uart_module.Init.BaudRate = UART_BAUDRATE_HMI;
	hmi_com.uart_module.Init.WordLength = UART_WORDLENGTH_8B;
	hmi_com.uart_module.Init.StopBits = UART_STOPBITS_1;
	hmi_com.uart_module.Init.Parity = UART_PARITY_NONE;
	hmi_com.uart_module.Init.Mode = UART_MODE_TX_RX;
	hmi_com.uart_module.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hmi_com.uart_module.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&hmi_com.uart_module) != HAL_OK)
	{
		Error_Handler();
	}
	uart_receives(&hmi_com, (char*)&hmi_com.rx_data);
}

static void uart_rs485_hw_init(void){
	rs485_com.uart_module.Instance = RS485_PORT_COM;
	rs485_com.uart_module.Init.BaudRate = UART_BAUDRATE_SLAVE;
	rs485_com.uart_module.Init.WordLength = UART_WORDLENGTH_8B;
	rs485_com.uart_module.Init.StopBits = UART_STOPBITS_1;
	rs485_com.uart_module.Init.Parity = UART_PARITY_NONE;
	rs485_com.uart_module.Init.Mode = UART_MODE_TX_RX;
	rs485_com.uart_module.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	rs485_com.uart_module.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&rs485_com.uart_module) != HAL_OK)
	{
		Error_Handler();
	}
	uart_receives(&rs485_com, (char*)&rs485_com.rx_data);
}

static void uart_debug_hw_init(void)
{
	debug_com.uart_module.Instance = DEBUG_PORT_COM;
	debug_com.uart_module.Init.BaudRate = UART_BAUDRATE_DEBUG;
	debug_com.uart_module.Init.WordLength = UART_WORDLENGTH_8B;
	debug_com.uart_module.Init.StopBits = UART_STOPBITS_1;
	debug_com.uart_module.Init.Parity = UART_PARITY_NONE;
	debug_com.uart_module.Init.Mode = UART_MODE_TX_RX;
	debug_com.uart_module.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	debug_com.uart_module.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&debug_com.uart_module) != HAL_OK)
	{
		Error_Handler();
	}
	uart_receives(&debug_com, (char*)&debug_com.rx_data);
}
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle){

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(uartHandle->Instance == RS485_PORT_COM)
	{
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, UART_RX_RS485_IRQN_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(uartHandle->Instance == HMI_PORT_COM)
	{
	    /* USART3 clock enable */
	    __HAL_RCC_USART3_CLK_ENABLE();

	    __HAL_RCC_GPIOB_CLK_ENABLE();
	    /**USART3 GPIO Configuration
	    PB10     ------> USART3_TX
	    PB11     ------> USART3_RX
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_10;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin = GPIO_PIN_11;
	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USART3 interrupt Init */
		HAL_NVIC_SetPriority(USART3_IRQn, UART_RX_HMI_IRQN_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
	}
	else if(uartHandle->Instance == DEBUG_PORT_COM)
	{
		/* Peripheral clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART2 setpriority*/
//		HAL_NVIC_SetPriority(USART2_IRQn, UART_RX_DEBUG_IRQN_PRIORITY, 0);
//		HAL_NVIC_EnableIRQ(USART2_IRQn); /// WARNING: Loss connected
	}
}

void uart_sends(UART_hw* p_hw, uint8_t* s){
	while(*s){
		HAL_UART_Transmit(&p_hw->uart_module, s, 1, 500);
		s++;
	}
}

char uart_receives(UART_hw* p_hw, char* s){
	HAL_UART_Receive_IT(&p_hw->uart_module, (uint8_t*)s, 1);
	return *s;
}

void USART1_IRQHandler(void){
	HAL_UART_IRQHandler(&rs485_com.uart_module);
	uart_receives(&rs485_com, (char*)&rs485_com.rx_data);
	if(rs485_com.receive_handle != NULL)
	{
		rs485_com.receive_handle(&rs485_com);
	}
}

void USART3_IRQHandler(void){
	HAL_UART_IRQHandler(&hmi_com.uart_module);
	uart_receives(&hmi_com, (char*)&hmi_com.rx_data);
	if(hmi_com.receive_handle != NULL)
	{
		hmi_com.receive_handle(&hmi_com);
	}
}

void USART2_IRQHandler(void){
	HAL_UART_IRQHandler(&debug_com.uart_module);
	uart_receives(&debug_com, (char*)&debug_com.rx_data);
	if(debug_com.receive_handle != NULL)
	{
		debug_com.receive_handle(&debug_com);
	}
}
