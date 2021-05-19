/*
 * test_active_node_id.c
 *
 *  Created on: May 17, 2021
 *      Author: KhanhDinh
 */

#include "rs485_master.h"
#include "cabinet_cell.h"
#include "board.h"
#include "cabinet_app.h"

Cabinet_App		selex_bss_app;
RS485_Master 	rs485m;
Cabinet			cabin[15];
static uint32_t sys_timestamp=0;
static uint32_t sys_tick_ms=0;
static uint32_t com_timestamp = 0;
uint8_t 		cnt, cnt_delay;
uint32_t 		time;

#define TEST_SLAVE_ID1	0
#define TEST_SLAVE_ID2	4


static void rs485_parse_slave_msg_handle_impl(RS485_Master* p_485m);

static void rs485_set_tx_mode(RS485_Master* p_485m){
	(void)p_485m;
	HAL_MAX485_SET_DIR_TX;
}

static void rs485_set_rx_mode(RS485_Master* p_485m){
	(void)p_485m;
	HAL_MAX485_SET_DIR_RX;
}

static void rs485_receive_handle_impl(UART_hw* p_hw){
	rs485m.is_new_msg = 1;
	if(rs485m.rx_index == 32){
		rs485m.rx_index = 0;
		return;
	}
	rs485m.rx_data[rs485m.rx_index] = p_hw->rx_data;
	rs485m.rx_index++;
}

static void hmi_receive_handle_impl(UART_hw* p_hw){
	selex_bss_app.is_new_msg = 1;
	if(selex_bss_app.rx_index == 32){
		selex_bss_app.rx_index = 0;
		return;
	}
	selex_bss_app.rx_data[selex_bss_app.rx_index] = p_hw->rx_data;
	selex_bss_app.rx_index++;
}

static void door_switch_on1(Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, TEST_SLAVE_ID1, SLAVE_DOOR, ACTIVE);
	rs485m.state = RS485_MASTER_ST_SEND_CMD;
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door_switch_on2(Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, TEST_SLAVE_ID2, SLAVE_DOOR, ACTIVE);
	rs485m.state = RS485_MASTER_ST_SEND_CMD;
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

int main(void){
	__disable_irq();
	HAL_Init();
	core_hw_init();
	uart_hw_init();
	max485_hw_init();
	sys_tick_ms=1000/SYSTICK_FREQ_Hz;
	sys_timestamp=0;
	cabinet_hw_init();

	rs485_master_init(&rs485m, 1, &cabinet_485_hw);
	rs485m.p_hw->uart_module = rs485_com.uart_module;
	rs485_com.receive_handle = rs485_receive_handle_impl;
	rs485m.set_transmit_mode = rs485_set_tx_mode;
	rs485m.set_receive_mode = rs485_set_rx_mode;
	rs485m.rx_index = 0;
	rs485m.parse_slave_msg_handle = rs485_parse_slave_msg_handle_impl;
	rs485m.state = RS485_MASTER_ST_IDLE;

	hmi_com.receive_handle = hmi_receive_handle_impl;

	selex_bss_app.bss.cabs = &cabin[0];
	selex_bss_app.bss.cabs[TEST_SLAVE_ID1].door.solenoid.sw_on = door_switch_on1;
	selex_bss_app.bss.cabs[TEST_SLAVE_ID2].door.solenoid.sw_on = door_switch_on2;

	__enable_irq();

	while(1);

}

void HAL_STATE_MACHINE_UPDATE_TICK(void){
	sys_timestamp+= sys_tick_ms;
}

static void rs485_parse_slave_msg_handle_impl(RS485_Master* p_485m){
	char* token = strtok((char*)p_485m->start_msg_index,",");
	if(p_485m->csv.id == string_to_long(token)){
		token = strtok(NULL, ",");
		switch (*token){
		case MASTER_WRITE:
			token = strtok(NULL, ",");
			if(*token == p_485m->csv.obj){
				p_485m->state = RS485_MASTER_ST_SEND_SYNC;
			}
			break;
		case MASTER_READ:
			token = strtok(NULL, ",");
			cabin[p_485m->csv.id].door.state = string_to_long(token);
			token = strtok(NULL, ",");
			cabin[p_485m->csv.id].cell_fan.state = string_to_long(token);
			token = strtok(NULL, ",");
			cabin[p_485m->csv.id].node_id_sw.state = string_to_long(token);
			token = strtok(NULL, ",");
			cabin[p_485m->csv.id].charger.state = string_to_long(token);
			token = strtok(NULL, ",");
			cabin[p_485m->csv.id].temp = string_to_long(token);
			p_485m->state = RS485_MASTER_ST_SUCCESS;
			break;
		default:
			break;
		}
	}
}

void TIM3_IRQHandler(void){
	com_timestamp+=1;
	if((com_timestamp%500) == 0){
		if(selex_bss_app.is_new_msg){
			cab_app_parse_hmi_msg(&selex_bss_app);
		}
	}
	else{
		rs485_master_update_state(&rs485m, com_timestamp);
	}
	HAL_TIM_IRQHandler(&io_scan_timer);
}

