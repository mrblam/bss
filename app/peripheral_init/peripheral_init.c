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

static void ntc_init(Cabinet_App* p_ca);
static void ntc_sensor_get_adc_value(NTC* p_ntc);

static void rs485_set_tx_mode(RS485_Master* p_485m);
static void rs485_set_rx_mode(RS485_Master* p_485m);
static void rs485_receive_handle_impl(UART_hw* p_hw);
static void hmi_receive_handle_impl(UART_hw* p_hw);
static void rs485_parse_slave_msg_handle_impl(RS485_Master* p_485m);


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
		p_ca->bss.cabs[cab_id].node_id_sw.sw_on = node_set_high[cab_id];
		p_ca->bss.cabs[cab_id].node_id_sw.sw_off = node_set_low[cab_id];
		p_ca->bss.cabs[cab_id].door.solenoid.sw_on = door_interface[cab_id];
		p_ca->bss.cabs[cab_id].door.io_state.get_io_state = ios_interface[cab_id];
	}

	rs485_master_init(&rs485m, 1, &cabinet_485_hw);
	rs485m.p_hw->uart_module = rs485_com.uart_module;
	rs485_com.receive_handle = rs485_receive_handle_impl;
	rs485m.set_transmit_mode = rs485_set_tx_mode;
	rs485m.set_receive_mode = rs485_set_rx_mode;
	rs485m.rx_index = 0;
	rs485m.state = RS485_MASTER_ST_IDLE;
	rs485m.parse_slave_msg_handle = rs485_parse_slave_msg_handle_impl;

	hmi_com.receive_handle = hmi_receive_handle_impl;

#if 0
	for(uint8_t cab_id=0;cab_id<p_ca->bss.cab_num;cab_id++){

		p_ca->bss.cabs[cab_id].node_id_sw.sw_on=node_set_high[cab_id];
		p_ca->bss.cabs[cab_id].node_id_sw.sw_off=node_set_low[cab_id];

		p_ca->bss.cabs[cab_id].door.solenoid.sw_on = door_interface[cab_id];
		p_ca->bss.cabs[cab_id].door.io_state.get_io_state = ios_interface[cab_id];

		p_ca->bss.cabs[cab_id].cell_fan.sw_on = cell_fan_on_interface[cab_id];
		p_ca->bss.cabs[cab_id].cell_fan.sw_off = cell_fan_off_interface[cab_id];
	}
#endif
	//ntc_init(p_ca);
}

static void ntc_init(Cabinet_App* p_ca){
	for(uint8_t cab_id = CAB1; cab_id < CAB15; cab_id++){
		p_ca->bss.cabs[cab_id].temp_ss->base.hw->adc_value = cab_temp[cab_id];
		ntc_set_lut(p_ca->bss.cabs[cab_id].temp_ss, ntc_lookups);
	}
}

/* ------------------------------------------------------------------------------ */

static void node_id1_sw_on(Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 0, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	while((rs485m.state == RS485_MASTER_ST_SEND_CMD) ||
			(rs485m.state == RS485_MASTER_ST_SEND_SYNC) ||
			(rs485m.state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(&rs485m);
	rs485m.state = RS485_MASTER_ST_IDLE;
}

static void node_id2_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 1, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	while((rs485m.state == RS485_MASTER_ST_SEND_CMD) ||
			(rs485m.state == RS485_MASTER_ST_SEND_SYNC) ||
			(rs485m.state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(&rs485m);
	rs485m.state = RS485_MASTER_ST_IDLE;
}

static void node_id3_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 2, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id4_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 3, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id5_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 4, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id6_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 5, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id7_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 6, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id8_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 7, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id9_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 8, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id10_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 9, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id11_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 10, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id12_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 11, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id13_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 12, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id14_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 13, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id15_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 14, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

/* ------------------------------------------------------------------------------ */

static void node_id1_sw_off(Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 0, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	while((rs485m.state == RS485_MASTER_ST_SEND_CMD) ||
			(rs485m.state == RS485_MASTER_ST_SEND_SYNC) ||
			(rs485m.state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(&rs485m);
	rs485m.state = RS485_MASTER_ST_IDLE;
}

static void node_id2_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 1, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	while((rs485m.state == RS485_MASTER_ST_SEND_CMD) ||
			(rs485m.state == RS485_MASTER_ST_SEND_SYNC) ||
			(rs485m.state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(&rs485m);
	rs485m.state = RS485_MASTER_ST_IDLE;
}

static void node_id3_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 2, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id4_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 3, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id5_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 4, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id6_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 5, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id7_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 6, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id8_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 7, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id9_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 8, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id10_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 9, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id11_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 10, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id12_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 11, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id13_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 12, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id14_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 13, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void node_id15_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_set_csv_data(&rs485m, 14, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_on( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_1_HIGH;
}

static void cell_fan2_switch_on( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_2_HIGH;
}

static void cell_fan3_switch_on( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_3_HIGH;
}

static void cell_fan4_switch_on( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_4_HIGH;
}

static void cell_fan5_switch_on( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_5_HIGH;
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
	CELL_FAN_1_LOW;
}

static void cell_fan2_switch_off( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_1_LOW;
}

static void cell_fan3_switch_off( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_1_LOW;
}

static void cell_fan4_switch_off( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_1_LOW;
}

static void cell_fan5_switch_off( Switch* p_sw){
	(void)p_sw;
	CELL_FAN_1_LOW;
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
	rs485_master_set_csv_data(&rs485m, 0, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	while((rs485m.state == RS485_MASTER_ST_SEND_CMD) ||
			(rs485m.state == RS485_MASTER_ST_SEND_SYNC) ||
			(rs485m.state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(&rs485m);
	rs485m.state = RS485_MASTER_ST_IDLE;
	return selex_bss_app.bss.cabs[0].door.state;
}

static DOOR_STATE door2_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 1, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	while((rs485m.state == RS485_MASTER_ST_SEND_CMD) ||
			(rs485m.state == RS485_MASTER_ST_SEND_SYNC) ||
			(rs485m.state == RS485_MASTER_ST_WAIT_CONFIRM));
	rs485_master_reset_buffer(&rs485m);
	rs485m.state = RS485_MASTER_ST_IDLE;
	return selex_bss_app.bss.cabs[1].door.state;
}

DOOR_STATE door3_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 2, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[2].door.state;
}

static DOOR_STATE door4_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 3, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[3].door.state;
}

static DOOR_STATE door5_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 4, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[4].door.state;
}

static DOOR_STATE door6_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 5, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[5].door.state;
}

static DOOR_STATE door7_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 6, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[6].door.state;
}

static DOOR_STATE door8_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 7, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[7].door.state;
}

static DOOR_STATE door9_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 8, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[8].door.state;
}

static DOOR_STATE door10_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 9, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[9].door.state;
}

static DOOR_STATE door11_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 10, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[10].door.state;
}

static DOOR_STATE door12_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 11, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[11].door.state;
}

static DOOR_STATE door13_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 12, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[12].door.state;
}

static DOOR_STATE door14_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 13, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[13].door.state;
}

static DOOR_STATE door15_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_set_csv_data(&rs485m, 14, 0, 0);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_SYNC;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
	return selex_bss_app.bss.cabs[14].door.state;
}

/*--------------------------------------------------------------------------------*/

static void door1_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 0, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door2_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 1, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door3_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 2, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door4_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 3, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door5_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 4, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door6_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 5, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door7_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 6, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door8_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 7, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door9_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 8, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door10_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 9, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door11_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 10, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door12_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 11, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door13_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 12, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door14_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 13, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

static void door15_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_set_csv_data(&rs485m, 14, SLAVE_DOOR, ACTIVE);
	if(rs485m.state == RS485_MASTER_ST_IDLE){
		rs485m.state = RS485_MASTER_ST_SEND_CMD;
	}
	//while(rs485m.state != RS485_MASTER_ST_SUCCESS);
}

/* ------------------------------------------------------------------------------ */

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
	if(p_hw->rx_data != '\0'){
		rs485m.rx_data[rs485m.rx_index] = p_hw->rx_data;
		rs485m.rx_index++;
	}
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
			selex_bss_app.bss.cabs[p_485m->csv.id].door.state = string_to_long(token);
			token = strtok(NULL, ",");
			selex_bss_app.bss.cabs[p_485m->csv.id].cell_fan.state = string_to_long(token);
			token = strtok(NULL, ",");
			selex_bss_app.bss.cabs[p_485m->csv.id].node_id_sw.state = string_to_long(token);
			token = strtok(NULL, ",");
			selex_bss_app.bss.cabs[p_485m->csv.id].charger.state = string_to_long(token);
			token = strtok(NULL, ",");
			selex_bss_app.bss.cabs[p_485m->csv.id].temp = string_to_long(token);
			p_485m->state = RS485_MASTER_ST_SUCCESS;
			break;
		default:
			break;
		}
	}
}
