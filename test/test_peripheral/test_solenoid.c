/*
 * test_solenoid.c
 *
 *  Created on: Apr 24, 2021
 *      Author: KhanhDinh
 */

#include "ioe.h"
#include "cabinet_app.h"
#include "peripheral_init.h"

IOE solenoid;
Cabinet_app selex_bss;
uint32_t cnt;

int main(void){
	__disable_irq();
	board_init();
	cab_app_init(&selex_bss);
	peripheral_init(&selex_bss);
	ioe_clear_all(&solenoid);
	//HAL_Delay(1);
	uint32_t cnt = 0;


#if 0
	for(uint8_t i = CAB1; i < CAB7; i++){
		cab_cell_open_door(selex_bss.cabin[i]);
	}
#endif
	while(1){
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) == GPIO_PIN_RESET){
			while(cnt < 2000000) cnt++;
			cnt = 0;
			if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) == GPIO_PIN_RESET){

			}

		}

#if 0
		while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_1, GPIO_PIN_SET);
			while(cnt < 100000) cnt++;
			cnt = 0;
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_1, GPIO_PIN_RESET);
			while(cnt < 100000) cnt++;
			cnt = 0;
		}

		while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_2) == GPIO_PIN_SET){
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_2, GPIO_PIN_SET);
			while(cnt < 100000) cnt++;
			cnt = 0;
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_2, GPIO_PIN_RESET);
			while(cnt < 100000) cnt++;
			cnt = 0;
		}

		while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_3) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_3, GPIO_PIN_SET);
			while(cnt < 100000) cnt++;
			cnt = 0;
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_3, GPIO_PIN_RESET);
			while(cnt < 100000) cnt++;
			cnt = 0;
		}

		while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_4) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_4, GPIO_PIN_SET);
			while(cnt < 100000) cnt++;
			cnt = 0;
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_4, GPIO_PIN_RESET);
			while(cnt < 100000) cnt++;
			cnt = 0;
		}

		while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_5) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_5, GPIO_PIN_SET);
			while(cnt < 100000) cnt++;
			cnt = 0;
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_5, GPIO_PIN_RESET);
			while(cnt < 100000) cnt++;
			cnt = 0;
		}

		while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_6) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_6, GPIO_PIN_SET);
			while(cnt < 100000) cnt++;
			cnt = 0;
			HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_6, GPIO_PIN_RESET);
			while(cnt < 100000) cnt++;
			cnt = 0;
		}
#endif
	}


}


