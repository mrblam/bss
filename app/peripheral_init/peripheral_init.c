/*
 * peripheral_init.c
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#include "peripheral_init.h"
#include "cabinet_app.h"
#include "cabinet_hw_hal.h"

static const uint16_t ntc_lookups[] = {
	    58747,//-10
	    55498,//-9
	    52451,//-8
	    49593,//-7
	    46910,//-6
	    44391,//-5
	    42024,//-4
	    39799,//-3
	    37708,//-2
	    35740,//-1
	    33889,//0
	    32146,//1
	    30504,//2
	    28957,//3
	    27499,//4
	    26124,//5
	    24827,//6
	    23603,//7
	    22447,//8
	    21356,//9
	    20324,//10
	    19350,//11
	    18428,//12
	    17556,//13
	    16731,//14
	    15951,//15
	    15211,//16
	    14511,//17
	    13847,//18
	    13219,//19
	    12622,//20
	    12056,//21
	    11520,//22
	    11010,//23
	    10526,//24
	    10067,//25
	    9630,//26
	    9215,//27
	    8821,//28
	    8446,//29
	    8089,//30
	    7750,//31
	    7426,//32
	    7119,//33
	    6826,//34
	    6546,//35
	    6280,//36
	    6026,//37
	    5785,//38
	    5554,//39
	    5334,//40
	    5124,//41
	    4923,//42
	    4731,//43
	    4548,//44
	    4374,//45
	    4207,//46
	    4047,//47
	    3894,//48
	    3748,//49
	    3609,//50
	    3475,//51
	    3347,//52
	    3225,//53
	    3107,//54
	    2995,//55
	    2887,//56
	    2784,//57
	    2685,//58
	    2590,//59
	    2499,//60
	    2412,//61
	    2329,//62
	    2248,//63
	    2171,//64
	    2097,//65
	    2026,//66
	    1958,//67
	    1892,//68
	    1829,//69
	    1769,//70
	    1710,//71
	    1654,//72
	    1601,//73
	    1549,//74
	    1499,//75
	    1451,//76
	    1405,//77
	    1360,//78
	    1318,//79
	    1276,//80
	    1237,//81
	    1198,//82
	    1162,//83
	    1126,//84
	    1092,//85
	    1059,//86
	    1027,//87
	    996,//88
	    966,//89
	    938,//90
	    910,//91
	    884,//92
	    858,//93
	    833,//94
	    809,//95
	    786,//96
	    763,//97
	    742,//98
	    721,//99
	    700,//100
};

static void node_id1_sw_on( Switch* p_cm);
static void node_id2_sw_on( Switch* p_cm);
static void node_id3_sw_on( Switch* p_cm);
static void node_id4_sw_on( Switch* p_cm);
static void node_id5_sw_on( Switch* p_cm);
static void node_id6_sw_on( Switch* p_cm);
static void node_id7_sw_on( Switch* p_cm);
static void node_id8_sw_on( Switch* p_cm);
static void node_id9_sw_on( Switch* p_cm);
static void node_id10_sw_on( Switch* p_cm);
static void node_id11_sw_on( Switch* p_cm);
static void node_id12_sw_on( Switch* p_cm);
static void node_id13_sw_on( Switch* p_cm);
static void node_id14_sw_on( Switch* p_cm);
static void node_id15_sw_on( Switch* p_cm);

static void node_id1_sw_off( Switch* p_cm);
static void node_id2_sw_off( Switch* p_cm);
static void node_id3_sw_off( Switch* p_cm);
static void node_id4_sw_off( Switch* p_cm);
static void node_id5_sw_off( Switch* p_cm);
static void node_id6_sw_off( Switch* p_cm);
static void node_id7_sw_off( Switch* p_cm);
static void node_id8_sw_off( Switch* p_cm);
static void node_id9_sw_off( Switch* p_cm);
static void node_id10_sw_off( Switch* p_cm);
static void node_id11_sw_off( Switch* p_cm);
static void node_id12_sw_off( Switch* p_cm);
static void node_id13_sw_off( Switch* p_cm);
static void node_id14_sw_off( Switch* p_cm);
static void node_id15_sw_off( Switch* p_cm);


static void door1_switch_on( Switch* p_sw);
static void door2_switch_on( Switch* p_sw);
static void door3_switch_on( Switch* p_sw);
static void door4_switch_on( Switch* p_sw);
static void door5_switch_on( Switch* p_sw);
static void door6_switch_on( Switch* p_sw);
static void door7_switch_on( Switch* p_sw);
static void door8_switch_on( Switch* p_sw);
static void door9_switch_on( Switch* p_sw);
static void door10_switch_on( Switch* p_sw);
static void door11_switch_on( Switch* p_sw);
static void door12_switch_on( Switch* p_sw);
static void door13_switch_on( Switch* p_sw);
static void door14_switch_on( Switch* p_sw);
static void door15_switch_on( Switch* p_sw);

static DOOR_STATE door1_get_state( IO_State* p_io);
static DOOR_STATE door2_get_state( IO_State* p_io);
static DOOR_STATE door3_get_state( IO_State* p_io);
static DOOR_STATE door4_get_state( IO_State* p_io);
static DOOR_STATE door5_get_state( IO_State* p_io);
static DOOR_STATE door6_get_state( IO_State* p_io);
static DOOR_STATE door7_get_state( IO_State* p_io);
static DOOR_STATE door8_get_state( IO_State* p_io);
static DOOR_STATE door9_get_state( IO_State* p_io);
static DOOR_STATE door10_get_state( IO_State* p_io);
static DOOR_STATE door11_get_state( IO_State* p_io);
static DOOR_STATE door12_get_state( IO_State* p_io);
static DOOR_STATE door13_get_state( IO_State* p_io);
static DOOR_STATE door14_get_state( IO_State* p_io);
static DOOR_STATE door15_get_state( IO_State* p_io);

static void cell_fan1_switch_on( Switch* p_sw);
static void cell_fan2_switch_on( Switch* p_sw);
static void cell_fan3_switch_on( Switch* p_sw);
static void cell_fan4_switch_on( Switch* p_sw);
static void cell_fan5_switch_on( Switch* p_sw);
static void cell_fan6_switch_on( Switch* p_sw);
static void cell_fan7_switch_on( Switch* p_sw);
static void cell_fan8_switch_on( Switch* p_sw);
static void cell_fan9_switch_on( Switch* p_sw);
static void cell_fan10_switch_on( Switch* p_sw);
static void cell_fan11_switch_on( Switch* p_sw);
static void cell_fan12_switch_on( Switch* p_sw);
static void cell_fan13_switch_on( Switch* p_sw);
static void cell_fan14_switch_on( Switch* p_sw);
static void cell_fan15_switch_on( Switch* p_sw);

static void cell_fan1_switch_off( Switch* p_sw);
static void cell_fan2_switch_off( Switch* p_sw);
static void cell_fan3_switch_off( Switch* p_sw);
static void cell_fan4_switch_off( Switch* p_sw);
static void cell_fan5_switch_off( Switch* p_sw);
static void cell_fan6_switch_off( Switch* p_sw);
static void cell_fan7_switch_off( Switch* p_sw);
static void cell_fan8_switch_off( Switch* p_sw);
static void cell_fan9_switch_off( Switch* p_sw);
static void cell_fan10_switch_off( Switch* p_sw);
static void cell_fan11_switch_off( Switch* p_sw);
static void cell_fan12_switch_off( Switch* p_sw);
static void cell_fan13_switch_off( Switch* p_sw);
static void cell_fan14_switch_off( Switch* p_sw);
static void cell_fan15_switch_off( Switch* p_sw);

static void mux_switch_channel1( NTC* p_ntc);
static void mux_switch_channel2( NTC* p_ntc);
static void mux_switch_channel3( NTC* p_ntc);
static void mux_switch_channel4( NTC* p_ntc);
static void mux_switch_channel5( NTC* p_ntc);
static void mux_switch_channel6( NTC* p_ntc);
static void mux_switch_channel7( NTC* p_ntc);
static void mux_switch_channel8( NTC* p_ntc);
static void mux_switch_channel9( NTC* p_ntc);
static void mux_switch_channel10( NTC* p_ntc);
static void mux_switch_channel11( NTC* p_ntc);
static void mux_switch_channel12( NTC* p_ntc);
static void mux_switch_channel13( NTC* p_ntc);
static void mux_switch_channel14( NTC* p_ntc);
static void mux_switch_channel15( NTC* p_ntc);

static void ntc_init(Cabinet_App* p_ca);
static void ntc_sensor_get_adc_value(NTC* p_ntc);

static sw_act door_interface[] = {door1_switch_on, door2_switch_on, door3_switch_on, door4_switch_on, door5_switch_on,
								door6_switch_on, door7_switch_on, door8_switch_on, door9_switch_on, door10_switch_on,
								door11_switch_on, door12_switch_on, door13_switch_on, door14_switch_on, door15_switch_on};

static ios_act ios_interface[] = {door1_get_state, door2_get_state, door3_get_state, door4_get_state, door5_get_state,
								door6_get_state, door7_get_state, door8_get_state, door9_get_state, door10_get_state,
								door11_get_state, door12_get_state, door13_get_state, door14_get_state, door15_get_state};

static sw_act cell_fan_on_interface[] = {cell_fan1_switch_on, cell_fan2_switch_on, cell_fan3_switch_on, cell_fan4_switch_on, cell_fan5_switch_on,
								cell_fan6_switch_on, cell_fan7_switch_on, cell_fan8_switch_on, cell_fan9_switch_on, cell_fan10_switch_on,
								cell_fan11_switch_on, cell_fan12_switch_on, cell_fan13_switch_on, cell_fan14_switch_on, cell_fan15_switch_on,};

static sw_act cell_fan_off_interface[] = {cell_fan1_switch_off, cell_fan2_switch_off, cell_fan3_switch_off, cell_fan4_switch_off, cell_fan5_switch_off,
								cell_fan6_switch_off, cell_fan7_switch_off, cell_fan8_switch_off, cell_fan9_switch_off, cell_fan10_switch_off,
								cell_fan11_switch_off, cell_fan12_switch_off, cell_fan13_switch_off, cell_fan14_switch_off, cell_fan15_switch_off};

static ntc_act mux_switch_channel_interface[] = {mux_switch_channel1, mux_switch_channel2, mux_switch_channel3, mux_switch_channel4, mux_switch_channel5,
								mux_switch_channel6, mux_switch_channel7, mux_switch_channel8, mux_switch_channel9, mux_switch_channel10,
								mux_switch_channel11, mux_switch_channel12, mux_switch_channel13, mux_switch_channel14, mux_switch_channel15};

static sw_act node_set_high[]={
		node_id1_sw_on,
		node_id2_sw_on,
		node_id3_sw_on,
		node_id4_sw_on,
		node_id5_sw_on,
		node_id6_sw_on,
		node_id7_sw_on,
		node_id8_sw_on,
		node_id9_sw_on,
		node_id10_sw_on,
		node_id11_sw_on,
		node_id12_sw_on,
		node_id13_sw_on,
		node_id14_sw_on,
		node_id15_sw_on
};

static sw_act node_set_low[]={
		node_id1_sw_off,
		node_id2_sw_off,
		node_id3_sw_off,
		node_id4_sw_off,
		node_id5_sw_off,
		node_id6_sw_off,
		node_id7_sw_off,
		node_id8_sw_off,
		node_id9_sw_off,
		node_id10_sw_off,
		node_id11_sw_off,
		node_id12_sw_off,
		node_id13_sw_off,
		node_id14_sw_off,
		node_id15_sw_off
};


void peripheral_init(Cabinet_App* p_ca){
	//ioe_init();
	for(uint8_t cab_id=0;cab_id<p_ca->bss.cab_num;cab_id++){

		p_ca->bss.cabs[cab_id].node_id_sw.sw_on=node_set_high[cab_id];
		p_ca->bss.cabs[cab_id].node_id_sw.sw_off=node_set_low[cab_id];

		p_ca->bss.cabs[cab_id].door.solenoid.sw_on = door_interface[cab_id];
		p_ca->bss.cabs[cab_id].door.io_state.get_io_state = ios_interface[cab_id];

		p_ca->bss.cabs[cab_id].cell_fan.sw_on = cell_fan_on_interface[cab_id];
		p_ca->bss.cabs[cab_id].cell_fan.sw_off = cell_fan_off_interface[cab_id];
	}
	//ntc_init(p_ca);
}

static void ntc_init(Cabinet_App* p_ca){
	p_ca->bss.cabs[CAB1].temp_ss->switch_channel = mux_switch_channel8;
	p_ca->bss.cabs[CAB2].temp_ss->switch_channel = mux_switch_channel7;
	p_ca->bss.cabs[CAB3].temp_ss->switch_channel = mux_switch_channel6;
	p_ca->bss.cabs[CAB4].temp_ss->switch_channel = mux_switch_channel5;
	p_ca->bss.cabs[CAB5].temp_ss->switch_channel = mux_switch_channel4;
	p_ca->bss.cabs[CAB6].temp_ss->switch_channel = mux_switch_channel3;
	p_ca->bss.cabs[CAB7].temp_ss->switch_channel = mux_switch_channel2;
	p_ca->bss.cabs[CAB8].temp_ss->switch_channel = mux_switch_channel1;
	p_ca->bss.cabs[CAB9].temp_ss->switch_channel = mux_switch_channel15;
	p_ca->bss.cabs[CAB10].temp_ss->switch_channel = mux_switch_channel14;
	p_ca->bss.cabs[CAB11].temp_ss->switch_channel = mux_switch_channel13;
	p_ca->bss.cabs[CAB12].temp_ss->switch_channel = mux_switch_channel12;
	p_ca->bss.cabs[CAB13].temp_ss->switch_channel = mux_switch_channel11;
	p_ca->bss.cabs[CAB14].temp_ss->switch_channel = mux_switch_channel10;
	p_ca->bss.cabs[CAB15].temp_ss->switch_channel = mux_switch_channel9;
	ntc_set_lut(p_ca->bss.cabs[CAB2].temp_ss, ntc_lookups);
#if 0
	for(uint8_t cab_id = CAB1; cab_id < CAB15; cab_id++){
		p_ca->cabin[cab_id]->temp_ss->switch_channel = mux_switch_channel_interface[cab_id];
		ntc_set_lut(p_ca->cabin[cab_id]->temp_ss, ntc_lookups);
	}
#endif
}

/* Used Node_ID 6-10 to config Node_ID pin 1,2,3,5,6 */
static void node_id1_sw_on(Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID6_HIGH;
}

static void node_id2_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID7_HIGH;
}

static void node_id3_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID8_HIGH;
}

static void node_id4_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID4_HIGH;
}

static void node_id5_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID9_HIGH;
}

static void node_id6_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID10_HIGH;
}

static void node_id7_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID7_HIGH;
}

static void node_id8_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID8_HIGH;
}

static void node_id9_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID9_HIGH;
}

static void node_id10_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID10_HIGH;
}

static void node_id11_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID11_HIGH;
}

static void node_id12_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID12_HIGH;
}

static void node_id13_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID13_HIGH;
}

static void node_id14_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID14_HIGH;
}

static void node_id15_sw_on( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID15_HIGH;
}


static void node_id1_sw_off(Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID6_LOW;
}

static void node_id2_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID7_LOW;
}

static void node_id3_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID8_LOW;
}

static void node_id4_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID4_LOW;
}

static void node_id5_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID9_LOW;
}

static void node_id6_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID10_LOW;
}

static void node_id7_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID7_LOW;
}

static void node_id8_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID8_LOW;
}

static void node_id9_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID9_LOW;
}

static void node_id10_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID10_LOW;
}

static void node_id11_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID11_LOW;
}

static void node_id12_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID12_LOW;
}

static void node_id13_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID13_LOW;
}

static void node_id14_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID14_LOW;
}

static void node_id15_sw_off( Switch* p_cm){
	(void)p_cm;
	HAL_NODE_ID15_LOW;
}



/*--------------------------------------------------------------------------------*/

static void mux_switch_channel1( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel2( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel3( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel4( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel5( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel6( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel7( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel8( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_RESET);
}

static void mux_switch_channel9( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel10( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel11( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel12( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel13( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel14( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

static void mux_switch_channel15( NTC* p_ntc){
	(void)p_ntc;
	HAL_GPIO_WritePin(MUX_PORT_S0, 		MUX_S0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MUX_PORT_S123, 	MUX_S3, GPIO_PIN_SET);
}

/*--------------------------------------------------------------------------------*/
/* Used Node_ID 11-15 to config Cell_fan pin 1,2,3,5,6 */
static void cell_fan1_switch_on( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_11, GPIO_PIN_SET);
}

static void cell_fan2_switch_on( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_12, GPIO_PIN_SET);
}

static void cell_fan3_switch_on( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_13, GPIO_PIN_SET);
}

static void cell_fan4_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB4);
}

static void cell_fan5_switch_on( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_14, GPIO_PIN_SET);
}

static void cell_fan6_switch_on( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_15, GPIO_PIN_SET);
}

static void cell_fan7_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB7);
}

static void cell_fan8_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB8);
}

static void cell_fan9_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB9);
}

static void cell_fan10_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB10);
}

static void cell_fan11_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB11);
}

static void cell_fan12_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB12);
}

static void cell_fan13_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB13);
}

static void cell_fan14_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB14);
}

static void cell_fan15_switch_on( Switch* p_sw){
	(void)p_sw;
	ioe_set_channel(&cell_fan, CAB15);
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_off( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_11, GPIO_PIN_RESET);
}

static void cell_fan2_switch_off( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_12, GPIO_PIN_RESET);
}

static void cell_fan3_switch_off( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_13, GPIO_PIN_RESET);
}

static void cell_fan4_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB4);
}

static void cell_fan5_switch_off( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_14, GPIO_PIN_RESET);
}

static void cell_fan6_switch_off( Switch* p_sw){
	(void)p_sw;
	HAL_GPIO_WritePin(CELL_NODE_ID_PORT, CELL_NODE_ID_15, GPIO_PIN_RESET);
}

static void cell_fan7_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB7);
}

static void cell_fan8_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB8);
}

static void cell_fan9_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB9);
}

static void cell_fan10_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB10);
}

static void cell_fan11_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB11);
}

static void cell_fan12_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB12);
}

static void cell_fan13_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB13);
}

static void cell_fan14_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB14);
}

static void cell_fan15_switch_off( Switch* p_sw){
	(void)p_sw;
	ioe_clear_channel(&cell_fan, CAB15);
}

/*--------------------------------------------------------------------------------*/

static DOOR_STATE door1_get_state( IO_State* p_io){
	(void) p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB1);
}

static DOOR_STATE door2_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB2);
}

DOOR_STATE door3_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB3);
}

static DOOR_STATE door4_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB4);
}

static DOOR_STATE door5_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB5);
}

static DOOR_STATE door6_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB6);
}

static DOOR_STATE door7_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB7);
}

static DOOR_STATE door8_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB8);
}

static DOOR_STATE door9_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB9);
}

static DOOR_STATE door10_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB10);
}

static DOOR_STATE door11_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB11);
}

static DOOR_STATE door12_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB12);
}

static DOOR_STATE door13_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB13);
}

static DOOR_STATE door14_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB14);
}

static DOOR_STATE door15_get_state( IO_State* p_io){
	(void)p_io;
	return (DOOR_STATE) HAL_DOOR_GET_STATE(CAB15);
}

/*--------------------------------------------------------------------------------*/

static void door1_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(0);
}

static void door2_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(1);
}

static void door3_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(2);
}

/* This door is fail */
static void door4_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(3);
}

static void door5_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(4);
}

static void door6_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(5);
}

/* This door is fail */
static void door7_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(6);
}

/* This door is fail */
static void door8_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(7);
}

/* This door is fail */
static void door9_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(8);
}

/* This door is fail */
static void door10_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(9);
}

/* This door is fail */
static void door11_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(10);
}

/* This door is fail */
static void door12_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(11);
}

/* This door is fail */
static void door13_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(12);
}

/* This door is fail */
static void door14_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(13);
}

/* This door is fail */
static void door15_switch_on( Switch* p_sw){
	(void)p_sw;
	door_sw_on(14);
}
