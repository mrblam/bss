/*
 * test_ntc.c
 *
 *  Created on: Apr 25, 2021
 *      Author: KhanhDinh
 */


/* NOTICE: the ntc_value was unstable, because PLLCLK of SYSCLKSource,
 * This problem can be solved by changed RCC_SYSCLKSOURCE_PLLCLK to RCC_SYSCLKSOURCE_HSI ???  */

#include "peripheral_init.h"

static Cabinet_app selex_bss;
uint8_t temp;
uint8_t temp1;

int main(void){
	__disable_irq();
	HAL_Init();
	core_hw_init();
	gpio_init();
	adc_hw_init();

	cab_app_init(&selex_bss);
	peripheral_init(&selex_bss);
	selex_bss.cabin[CAB2]->temp_ss->switch_channel(selex_bss.cabin[CAB2]->temp_ss);
	__enable_irq();
	while(1){

	}
}
#if 0
void ADC1_2_IRQHandler(void){
	HAL_ADC_IRQHandler(&ntc.adc_module);
	ntc.adc_value = HAL_ADC_GetValue(&ntc.adc_module);
	if(ntc.adc_value){
		ntc_update_temp(selex_bss.cabin[CAB2]->temp_ss);
		temp = selex_bss.cabin[CAB2]->temp_ss->temp;
		temp1 = temp;
	}
}
#endif

void HAL_STATE_MACHINE_UPDATE_TICK(void){

}

