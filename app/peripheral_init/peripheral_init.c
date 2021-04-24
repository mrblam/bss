/*
 * peripheral_init.c
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#include "peripheral_init.h"
#include "cabinet_app.h"

static void door1_switch_on(__attribute__((unused)) Switch* p_sw);
static void door2_switch_on(__attribute__((unused)) Switch* p_sw);
static void door3_switch_on(__attribute__((unused)) Switch* p_sw);
static void door4_switch_on(__attribute__((unused)) Switch* p_sw);
static void door5_switch_on(__attribute__((unused)) Switch* p_sw);
static void door6_switch_on(__attribute__((unused)) Switch* p_sw);
static void door7_switch_on(__attribute__((unused)) Switch* p_sw);
static void door8_switch_on(__attribute__((unused)) Switch* p_sw);
static void door9_switch_on(__attribute__((unused)) Switch* p_sw);
static void door10_switch_on(__attribute__((unused)) Switch* p_sw);
static void door11_switch_on(__attribute__((unused)) Switch* p_sw);
static void door12_switch_on(__attribute__((unused)) Switch* p_sw);
static void door13_switch_on(__attribute__((unused)) Switch* p_sw);
static void door14_switch_on(__attribute__((unused)) Switch* p_sw);
static void door15_switch_on(__attribute__((unused)) Switch* p_sw);

static DOOR_STATE door1_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door2_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door3_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door4_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door5_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door6_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door7_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door8_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door9_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door10_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door11_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door12_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door13_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door14_get_state(__attribute__((unused)) IO_State* p_io);
static DOOR_STATE door15_get_state(__attribute__((unused)) IO_State* p_io);

static void cell_fan1_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan2_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan3_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan4_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan5_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan6_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan7_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan8_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan9_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan10_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan11_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan12_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan13_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan14_switch_on(__attribute__((unused)) Switch* p_sw);
static void cell_fan15_switch_on(__attribute__((unused)) Switch* p_sw);

static void cell_fan1_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan2_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan3_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan4_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan5_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan6_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan7_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan8_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan9_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan10_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan11_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan12_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan13_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan14_switch_off(__attribute__((unused)) Switch* p_sw);
static void cell_fan15_switch_off(__attribute__((unused)) Switch* p_sw);

static void mux_switch_channel1(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel2(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel3(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel4(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel5(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel6(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel7(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel8(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel9(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel10(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel11(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel12(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel13(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel14(__attribute__((unused)) NTC* p_ntc);
static void mux_switch_channel15(__attribute__((unused)) NTC* p_ntc);

static void node_id1_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id2_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id3_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id4_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id5_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id6_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id7_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id8_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id9_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id10_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id11_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id12_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id13_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id14_switch_on(__attribute__((unused)) CAN_master* p_cm);
static void node_id15_switch_on(__attribute__((unused)) CAN_master* p_cm);

static void node_id1_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id2_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id3_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id4_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id5_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id6_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id7_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id8_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id9_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id10_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id11_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id12_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id13_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id14_switch_off(__attribute__((unused)) CAN_master* p_cm);
static void node_id15_switch_off(__attribute__((unused)) CAN_master* p_cm);

static void door_switch_init(Cabinet_app* p_ca);
static void cell_fan_init(Cabinet_app* p_ca);
static void ntc_init(Cabinet_app* p_ca);
static void ntc_sensor_get_adc_value(NTC* p_ntc);

static sw_act door_interface[15] = {door1_switch_on, door2_switch_on, door3_switch_on, door4_switch_on, door5_switch_on,
								door6_switch_on, door7_switch_on, door8_switch_on, door9_switch_on, door10_switch_on,
								door11_switch_on, door12_switch_on, door13_switch_on, door14_switch_on, door15_switch_on};

static ios_act ios_interface[15] = {door1_get_state, door2_get_state, door3_get_state, door4_get_state, door5_get_state,
								door6_get_state, door7_get_state, door8_get_state, door9_get_state, door10_get_state,
								door11_get_state, door12_get_state, door13_get_state, door14_get_state, door15_get_state};

static sw_act cell_fan_on_interface[15] = {cell_fan1_switch_on, cell_fan2_switch_on, cell_fan3_switch_on, cell_fan4_switch_on, cell_fan5_switch_on,
								cell_fan6_switch_on, cell_fan7_switch_on, cell_fan8_switch_on, cell_fan9_switch_on, cell_fan10_switch_on,
								cell_fan11_switch_on, cell_fan12_switch_on, cell_fan13_switch_on, cell_fan14_switch_on, cell_fan15_switch_on,};

static sw_act cell_fan_off_interface[15] = {cell_fan1_switch_off, cell_fan2_switch_off, cell_fan3_switch_off, cell_fan4_switch_off, cell_fan5_switch_off,
								cell_fan6_switch_off, cell_fan7_switch_off, cell_fan8_switch_off, cell_fan9_switch_off, cell_fan10_switch_off,
								cell_fan11_switch_off, cell_fan12_switch_off, cell_fan13_switch_off, cell_fan14_switch_off, cell_fan15_switch_off};

static ntc_act mux_switch_channel_interface[15] = {mux_switch_channel1, mux_switch_channel2, mux_switch_channel3, mux_switch_channel4, mux_switch_channel5,
								mux_switch_channel6, mux_switch_channel7, mux_switch_channel8, mux_switch_channel9, mux_switch_channel10,
								mux_switch_channel11, mux_switch_channel12, mux_switch_channel13, mux_switch_channel14, mux_switch_channel15};

void peripheral_init(Cabinet_app* p_ca){
	door_switch_init(p_ca);
	cell_fan_init(p_ca);
	ntc_init(p_ca);
}

static void door_switch_init(Cabinet_app* p_ca){
	for(uint8_t cab_id = CAB1; cab_id <= CAB15; cab_id++){
		p_ca->cabin[cab_id]->door->solenoid->sw_on = door_interface[cab_id];
		p_ca->cabin[cab_id]->door->io_state->get_io_state = ios_interface[cab_id];
	}
}

static void cell_fan_init(Cabinet_app* p_ca){
	for(uint8_t cab_id = CAB1; cab_id <= CAB15; cab_id++){
		p_ca->cabin[cab_id]->cell_fan->sw_on = cell_fan_on_interface[cab_id];
		p_ca->cabin[cab_id]->cell_fan->sw_off = cell_fan_off_interface[cab_id];
	}
}

static void ntc_init(Cabinet_app* p_ca){
	for(uint8_t cab_id = CAB1; cab_id <= CAB15; cab_id++){
		p_ca->cabin[cab_id]->temp_ss->switch_channel = mux_switch_channel_interface[cab_id];
		ntc_set_lut(p_ca->cabin[cab_id]->temp_ss, ntc_lookups);
	}
}

static void node_id1_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_1, GPIO_PIN_SET);
}

static void node_id2_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_2, GPIO_PIN_SET);
}

static void node_id3_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_3, GPIO_PIN_SET);
}

static void node_id4_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_4, GPIO_PIN_SET);
}

static void node_id5_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_5, GPIO_PIN_SET);
}

static void node_id6_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_6, GPIO_PIN_SET);
}

static void node_id7_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_7, GPIO_PIN_SET);
}

static void node_id8_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_8, GPIO_PIN_SET);
}

static void node_id9_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_9, GPIO_PIN_SET);
}

static void node_id10_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_10, GPIO_PIN_SET);
}

static void node_id11_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_11, GPIO_PIN_SET);
}

static void node_id12_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_12, GPIO_PIN_SET);
}

static void node_id13_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_13, GPIO_PIN_SET);
}

static void node_id14_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_14, GPIO_PIN_SET);
}

static void node_id15_switch_on(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_15, GPIO_PIN_SET);
}

/*--------------------------------------------------------------------------------*/

static void node_id1_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_1, GPIO_PIN_RESET);
}

static void node_id2_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_2, GPIO_PIN_RESET);
}

static void node_id3_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_3, GPIO_PIN_RESET);
}

static void node_id4_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_4, GPIO_PIN_RESET);
}

static void node_id5_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_5, GPIO_PIN_RESET);
}

static void node_id6_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_6, GPIO_PIN_RESET);
}

static void node_id7_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_7, GPIO_PIN_RESET);
}

static void node_id8_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_8, GPIO_PIN_RESET);
}

static void node_id9_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_9, GPIO_PIN_RESET);
}

static void node_id10_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_10, GPIO_PIN_RESET);
}

static void node_id11_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_11, GPIO_PIN_RESET);
}

static void node_id12_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_12, GPIO_PIN_RESET);
}

static void node_id13_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_13, GPIO_PIN_RESET);
}

static void node_id14_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_14, GPIO_PIN_RESET);
}

static void node_id15_switch_off(__attribute__((unused)) CAN_master* p_cm){
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_15, GPIO_PIN_RESET);
}

/*--------------------------------------------------------------------------------*/

static void mux_switch_channel1(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel2(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel3(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel4(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel5(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel6(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel7(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel8(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel9(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel10(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel11(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel12(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel13(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel14(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel15(__attribute__((unused)) NTC* p_ntc){
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}
/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB1);
}

static void cell_fan2_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB2);
}

static void cell_fan3_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB3);
}

static void cell_fan4_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB4);
}

static void cell_fan5_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB5);
}

static void cell_fan6_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB6);
}

static void cell_fan7_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB7);
}

static void cell_fan8_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB8);
}

static void cell_fan9_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB9);
}

static void cell_fan10_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB10);
}

static void cell_fan11_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB11);
}

static void cell_fan12_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB12);
}

static void cell_fan13_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB13);
}

static void cell_fan14_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB14);
}

static void cell_fan15_switch_on(__attribute__((unused)) Switch* p_sw){
	ioe_set_channel(&cell_fan, CAB15);
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB1);
}

static void cell_fan2_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB2);
}

static void cell_fan3_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB3);
}

static void cell_fan4_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB4);
}

static void cell_fan5_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB5);
}

static void cell_fan6_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB6);
}

static void cell_fan7_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB7);
}

static void cell_fan8_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB8);
}

static void cell_fan9_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB9);
}

static void cell_fan10_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB10);
}

static void cell_fan11_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB11);
}

static void cell_fan12_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB12);
}

static void cell_fan13_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB13);
}

static void cell_fan14_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB14);
}

static void cell_fan15_switch_off(__attribute__((unused)) Switch* p_sw){
	ioe_clear_channel(&cell_fan, CAB15);
}

/*--------------------------------------------------------------------------------*/

static DOOR_STATE door1_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door2_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_2) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_2) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

DOOR_STATE door3_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_3) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_3) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door4_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_4) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_4) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door5_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_5) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_5) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door6_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_6) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_6) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door7_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_7) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_7) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door8_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_8) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_8) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door9_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_9) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_9) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door10_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_10) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_10) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door11_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_11) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_11) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door12_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_12) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_12) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door13_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_13) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_13) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door14_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_14) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_14) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

static DOOR_STATE door15_get_state(__attribute__((unused)) IO_State* p_io){
	if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_15) ==  GPIO_PIN_RESET){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_15) == GPIO_PIN_RESET){
			return DOOR_ST_CLOSE;
		}
	}
	else return DOOR_ST_OPEN;
	return 0;
}

/*--------------------------------------------------------------------------------*/

static void door1_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_1) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB1);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB1);
		HAL_Delay(500);
	}
}

static void door2_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_2) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB2);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB2);
		HAL_Delay(500);
	}
}

static void door3_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_3) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB3);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB3);
		HAL_Delay(500);
	}
}

static void door4_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT1_4, DOOR_ST_4) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB4);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB4);
		HAL_Delay(500);
	}
}

static void door5_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_5) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB5);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB5);
		HAL_Delay(500);
	}
}

static void door6_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_6) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB6);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB6);
		HAL_Delay(500);
	}
}

static void door7_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_7) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB7);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB7);
		HAL_Delay(500);
	}
}

static void door8_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_8) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB8);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB8);
		HAL_Delay(500);
	}
}

static void door9_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_9) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB9);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB9);
		HAL_Delay(500);
	}
}

static void door10_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_10) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB10);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB10);
		HAL_Delay(500);
	}
}

static void door11_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_11) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB11);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB11);
		HAL_Delay(500);
	}
}

static void door12_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_12) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB12);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB12);
		HAL_Delay(500);
	}
}

static void door13_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_13) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB13);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB13);
		HAL_Delay(500);
	}
}

static void door14_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_14) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB14);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB14);
		HAL_Delay(500);
	}
}

static void door15_switch_on(__attribute__((unused)) Switch* p_sw){
	while(HAL_GPIO_ReadPin(DOOR_ST_PORT5_16, DOOR_ST_15) == GPIO_PIN_RESET){
		ioe_set_channel(&solenoid, CAB15);
		HAL_Delay(500);
		ioe_clear_channel(&solenoid, CAB15);
		HAL_Delay(500);
	}
}






