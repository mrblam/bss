/*
 * test_master_command.c
 *
 *  Created on: May 11, 2021
 *      Author: KhanhDinh
 */

#include "rs485_master.h"
#include "cabinet_cell.h"
#include "board.h"
#include "cabinet_app.h"

Cabinet_App	selex_bss_app;
RS485_Master rs485m;
Cabinet			cab1;
static uint32_t sys_timestamp=0;
static uint32_t sys_tick_ms=0;
uint8_t cnt, cnt_delay;
uint32_t time;

static void rs485_parse_sync_msg_handle(RS485_Master* p_485m);

static void rs485_set_tx_mode(RS485_Master* p_485m){
	(void)p_485m;
	HAL_MAX485_SET_DIR_TX;
}

static void rs485_set_rx_mode(RS485_Master* p_485m){
	(void)p_485m;
	HAL_MAX485_SET_DIR_RX;
}

static void uart_receive_handle_impl(UART_hw* p_hw){
	rs485m.is_new_msg = 1;
	if(rs485m.rx_index < 32){
		rs485m.rx_data[rs485m.rx_index] = p_hw->rx_data;
		rs485m.rx_index++;
	}
	else rs485m.rx_index = 0;
}

int main(void){
	__disable_irq();
	HAL_Init();
	core_hw_init();
	uart_hw_init();
	max485_hw_init();
	sys_tick_ms=1000/SYSTICK_FREQ_Hz;
	sys_timestamp=0;

	rs485_master_init(&rs485m, 1, &cabinet_485_hw);
	rs485m.p_hw->uart_module = power_sys_port.uart_module;
	power_sys_port.receive_handle = uart_receive_handle_impl;
	rs485m.set_transmit_mode = rs485_set_tx_mode;
	rs485m.set_receive_mode = rs485_set_rx_mode;
	rs485m.rx_index = 0;
	rs485m.parse_sync_msg_handle = rs485_parse_sync_msg_handle;

	rs485m.state = RS485_MASTER_ST_IDLE;
	//cnt = 1;
	uart_receives(&power_sys_port, (char*)&power_sys_port.rx_data);
	rs485m.state = RS485_MASTER_ST_SEND_CMD;
	__enable_irq();

	while(1);

}

void HAL_STATE_MACHINE_UPDATE_TICK(void){
	sys_timestamp+= sys_tick_ms;
	if(cnt == 0){
		rs485_master_process_switch_command(&rs485m, 3, 'D', 1, sys_timestamp);
		if(rs485m.state == RS485_MASTER_ST_SUCCESS){
			rs485m.state = RS485_MASTER_ST_IDLE;
			for(uint8_t i = 0; i<32; i++){
				rs485m.rx_data[i] = 0;
				rs485m.tx_data[i] = 0;
			}
			rs485m.rx_index = 0;
			//cnt_delay = 1;
			//cnt = 2;
			time = sys_timestamp + 1000;

		}
	}
	else if(cnt == 1){
		rs485_master_process_switch_command(&rs485m, 0, 'D', 1, sys_timestamp);
		if(rs485m.state == RS485_MASTER_ST_SUCCESS){
			rs485m.state = RS485_MASTER_ST_IDLE;
			for(uint8_t i = 0; i<32; i++){
				rs485m.rx_data[i] = 0;
				rs485m.tx_data[i] = 0;
			}
			rs485m.rx_index = 0;
			//cnt_delay = 1;
			//cnt = 3;
			time = sys_timestamp + 1000;

		}
	}
	else if(cnt_delay == 1){
		if(time < sys_timestamp){
			if(cnt == 3) {
				cnt = 0;
				cnt_delay = 0;
			}
			else if(cnt == 2) {
				cnt = 1;
				cnt_delay = 0;
			}
		}
	}
}

static void rs485_parse_sync_msg_handle(RS485_Master* p_485m){
	if(rs485_master_check_valid_msg(p_485m)){
		char* token = strtok((char*)p_485m->start_msg_index,",");
		if(p_485m->csv.id == string_to_long(token)){
			token = strtok(NULL, ",");
				switch (*token){
				case 'W':
					token = strtok(NULL, ",");
					if(*token == p_485m->csv.obj){
					p_485m->state = RS485_MASTER_ST_SEND_SYNC;
					break;
				case 'R':
					token = strtok(NULL, ",");
					cab1.door.state = string_to_long(token);
					token = strtok(NULL, ",");
					cab1.cell_fan.state = string_to_long(token);
					token = strtok(NULL, ",");
					cab1.node_id_sw.state = string_to_long(token);
					token = strtok(NULL, ",");
					cab1.charger.state = string_to_long(token);
					token = strtok(NULL, ",");
					cab1.temp = string_to_long(token);
					p_485m->state = RS485_MASTER_ST_SUCCESS;
					break;
				default:
					break;
				}
			}
		}
	}
}

void USART1_IRQHandler(void){
	HAL_UART_IRQHandler(&power_sys_port.uart_module);
	uart_receives(&power_sys_port, (char*)&power_sys_port.rx_data);
	if(power_sys_port.receive_handle != NULL){
		power_sys_port.receive_handle(&power_sys_port);
	}
}
