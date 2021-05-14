#include "uart_hw.h"

UART_hw sim_port;
UART_hw hmi_port;
UART_hw power_sys_port;

static void uart_hmi_hw_init(void);
static void uart_power_hw_init(void);
static void uart_sim_hw_init(void);

void uart_hw_init(void){
	uart_hmi_hw_init();
	uart_power_hw_init();
	uart_sim_hw_init();
}

static void uart_hmi_hw_init(void){
	hmi_port.uart_module.Instance = HMI_PORT_COM;
	hmi_port.uart_module.Init.BaudRate = UART_BAUDRATE;
	hmi_port.uart_module.Init.WordLength = UART_WORDLENGTH_8B;
	hmi_port.uart_module.Init.StopBits = UART_STOPBITS_1;
	hmi_port.uart_module.Init.Parity = UART_PARITY_NONE;
	hmi_port.uart_module.Init.Mode = UART_MODE_TX_RX;
	hmi_port.uart_module.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hmi_port.uart_module.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&hmi_port.uart_module) != HAL_OK)
	{
		Error_Handler();
	}
}

static void uart_power_hw_init(void){
	power_sys_port.uart_module.Instance = POWER_SYS_PORT_COM;
	power_sys_port.uart_module.Init.BaudRate = UART_BAUDRATE;
	power_sys_port.uart_module.Init.WordLength = UART_WORDLENGTH_8B;
	power_sys_port.uart_module.Init.StopBits = UART_STOPBITS_1;
	power_sys_port.uart_module.Init.Parity = UART_PARITY_NONE;
	power_sys_port.uart_module.Init.Mode = UART_MODE_TX_RX;
	power_sys_port.uart_module.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	power_sys_port.uart_module.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&power_sys_port.uart_module) != HAL_OK)
	{
		Error_Handler();
	}
}

static void uart_sim_hw_init(void){
	sim_port.uart_module.Instance = SIM_PORT_COM;
	sim_port.uart_module.Init.BaudRate = UART_BAUDRATE;
	sim_port.uart_module.Init.WordLength = UART_WORDLENGTH_8B;
	sim_port.uart_module.Init.StopBits = UART_STOPBITS_1;
	sim_port.uart_module.Init.Parity = UART_PARITY_NONE;
	sim_port.uart_module.Init.Mode = UART_MODE_TX_RX;
	sim_port.uart_module.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	sim_port.uart_module.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&sim_port.uart_module) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle){

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(uartHandle->Instance == SIM_PORT_COM){
		/* USART2 clock enable */
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

		/* USART2 interrupt Init */
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
	else if(uartHandle->Instance == POWER_SYS_PORT_COM){
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
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(uartHandle->Instance == HMI_PORT_COM){
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
#if 0
void USART1_IRQHandler(void){
	HAL_UART_IRQHandler(&power_sys_port.uart_module);
	uart_receives(&power_sys_port, (char*)&power_sys_port.rx_data);
	if(power_sys_port.receive_handle != NULL){
		power_sys_port.receive_handle(&power_sys_port);
	}
}
#endif
