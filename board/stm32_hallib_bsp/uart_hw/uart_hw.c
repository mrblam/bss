#include "uart_hw.h"

UART_HandleTypeDef uart_sim;
UART_HandleTypeDef uart_power;
UART_HandleTypeDef uart_hmi;

static void uart_hmi_hw_init(void);
static void uart_power_hw_init(void);
static void uart_sim_hw_init(void);

void uart_hw_init(void){
	uart_hmi_hw_init();
	//uart_power_hw_init();
	//uart_sim_hw_init();
}

static void uart_hmi_hw_init(void){
	uart_hmi.Instance = UART_HMI;
	uart_hmi.Init.BaudRate = 115200;
	uart_hmi.Init.WordLength = UART_WORDLENGTH_8B;
	uart_hmi.Init.StopBits = UART_STOPBITS_1;
	uart_hmi.Init.Parity = UART_PARITY_NONE;
	uart_hmi.Init.Mode = UART_MODE_TX_RX;
	uart_hmi.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_hmi.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&uart_hmi) != HAL_OK)
	{
		Error_Handler();
	}
}

static void uart_power_hw_init(void){
	uart_power.Instance = UART_POWER;
	uart_power.Init.BaudRate = 115200;
	uart_power.Init.WordLength = UART_WORDLENGTH_8B;
	uart_power.Init.StopBits = UART_STOPBITS_1;
	uart_power.Init.Parity = UART_PARITY_NONE;
	uart_power.Init.Mode = UART_MODE_TX_RX;
	uart_power.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_power.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&uart_power) != HAL_OK)
	{
		Error_Handler();
	}
}

static void uart_sim_hw_init(void){
	uart_sim.Instance = UART_SIM;
	uart_sim.Init.BaudRate = 115200;
	uart_sim.Init.WordLength = UART_WORDLENGTH_8B;
	uart_sim.Init.StopBits = UART_STOPBITS_1;
	uart_sim.Init.Parity = UART_PARITY_NONE;
	uart_sim.Init.Mode = UART_MODE_TX_RX;
	uart_sim.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_sim.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&uart_sim) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(uartHandle->Instance==UART_SIM){
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
	else if(uartHandle->Instance==UART_POWER){
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
	}
	else if(uartHandle->Instance==UART_HMI){
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
