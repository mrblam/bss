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
static void node_id16_sw_on( Switch* p_cm);
static void node_id17_sw_on( Switch* p_cm);
static void node_id18_sw_on( Switch* p_cm);
static void node_id19_sw_on( Switch* p_cm);
static void node_id20_sw_on( Switch* p_cm);

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
static void node_id16_sw_off( Switch* p_cm);
static void node_id17_sw_off( Switch* p_cm);
static void node_id18_sw_off( Switch* p_cm);
static void node_id19_sw_off( Switch* p_cm);
static void node_id20_sw_off( Switch* p_cm);

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
static void door16_switch_on( Switch* p_sw);
static void door17_switch_on( Switch* p_sw);
static void door18_switch_on( Switch* p_sw);
static void door19_switch_on( Switch* p_sw);
static void door20_switch_on( Switch* p_sw);

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
static DOOR_STATE door16_get_state( IO_State* p_io);
static DOOR_STATE door17_get_state( IO_State* p_io);
static DOOR_STATE door18_get_state( IO_State* p_io);
static DOOR_STATE door19_get_state( IO_State* p_io);
static DOOR_STATE door20_get_state( IO_State* p_io);

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
static void cell_fan16_switch_on( Switch* p_sw);
static void cell_fan17_switch_on( Switch* p_sw);
static void cell_fan18_switch_on( Switch* p_sw);
static void cell_fan19_switch_on( Switch* p_sw);
static void cell_fan20_switch_on( Switch* p_sw);

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
static void cell_fan16_switch_on( Switch* p_sw);
static void cell_fan17_switch_on( Switch* p_sw);
static void cell_fan18_switch_on( Switch* p_sw);
static void cell_fan19_switch_on( Switch* p_sw);
static void cell_fan20_switch_on( Switch* p_sw);

static void charger1_sw_on(Switch* p_sw);
static void charger2_sw_on(Switch* p_sw);
static void charger3_sw_on(Switch* p_sw);
static void charger4_sw_on(Switch* p_sw);
static void charger5_sw_on(Switch* p_sw);
static void charger6_sw_on(Switch* p_sw);
static void charger7_sw_on(Switch* p_sw);
static void charger8_sw_on(Switch* p_sw);
static void charger9_sw_on(Switch* p_sw);
static void charger10_sw_on(Switch* p_sw);
static void charger11_sw_on(Switch* p_sw);
static void charger12_sw_on(Switch* p_sw);
static void charger13_sw_on(Switch* p_sw);
static void charger14_sw_on(Switch* p_sw);
static void charger15_sw_on(Switch* p_sw);
static void charger16_sw_on(Switch* p_sw);
static void charger17_sw_on(Switch* p_sw);
static void charger18_sw_on(Switch* p_sw);
static void charger19_sw_on(Switch* p_sw);
static void charger20_sw_on(Switch* p_sw);

static void charger1_sw_off(Switch* p_sw);
static void charger2_sw_off(Switch* p_sw);
static void charger3_sw_off(Switch* p_sw);
static void charger4_sw_off(Switch* p_sw);
static void charger5_sw_off(Switch* p_sw);
static void charger6_sw_off(Switch* p_sw);
static void charger7_sw_off(Switch* p_sw);
static void charger8_sw_off(Switch* p_sw);
static void charger9_sw_off(Switch* p_sw);
static void charger10_sw_off(Switch* p_sw);
static void charger11_sw_off(Switch* p_sw);
static void charger12_sw_off(Switch* p_sw);
static void charger13_sw_off(Switch* p_sw);
static void charger14_sw_off(Switch* p_sw);
static void charger15_sw_off(Switch* p_sw);
static void charger16_sw_off(Switch* p_sw);
static void charger17_sw_off(Switch* p_sw);
static void charger18_sw_off(Switch* p_sw);
static void charger19_sw_off(Switch* p_sw);
static void charger20_sw_off(Switch* p_sw);



static void ntc_init(Cabinet_App* p_ca);
static void ntc_sensor_get_adc_value(NTC* p_ntc);

static void rs485_set_tx_mode(RS485_Master* p_485m);
static void rs485_set_rx_mode(RS485_Master* p_485m);
static void rs485_receive_handle_impl(UART_hw* p_hw);
static void hmi_receive_handle_impl(UART_hw* p_hw);
static void rs485_parse_slave_msg_handle_impl(RS485_Master* p_485m);
static void cab_cell_data_serialze_impl(Cabinet* p_cc, char* buff);

static void can_master_rpdo_process_impl(const CAN_master* const p_cm);

static sw_act door_sw_interface[] = {door1_switch_on, door2_switch_on, door3_switch_on, door4_switch_on, door5_switch_on,
								door6_switch_on, door7_switch_on, door8_switch_on, door9_switch_on, door10_switch_on,
								door11_switch_on, door12_switch_on, door13_switch_on, door14_switch_on, door15_switch_on,
								door16_switch_on, door17_switch_on, door18_switch_on, door19_switch_on, door20_switch_on};

static ios_act door_state_interface[] = {door1_get_state, door2_get_state, door3_get_state, door4_get_state, door5_get_state,
								door6_get_state, door7_get_state, door8_get_state, door9_get_state, door10_get_state,
								door11_get_state, door12_get_state, door13_get_state, door14_get_state, door15_get_state,
								door16_get_state, door17_get_state, door18_get_state, door19_get_state, door20_get_state};

static sw_act cell_fan_on_interface[] = {cell_fan1_switch_on, cell_fan2_switch_on, cell_fan3_switch_on, cell_fan4_switch_on, cell_fan5_switch_on,
								cell_fan6_switch_on, cell_fan7_switch_on, cell_fan8_switch_on, cell_fan9_switch_on, cell_fan10_switch_on,
								cell_fan11_switch_on, cell_fan12_switch_on, cell_fan13_switch_on, cell_fan14_switch_on, cell_fan15_switch_on,
								cell_fan16_switch_on, cell_fan17_switch_on, cell_fan18_switch_on, cell_fan19_switch_on, cell_fan20_switch_on};

static sw_act cell_fan_off_interface[] = {cell_fan1_switch_off, cell_fan2_switch_off, cell_fan3_switch_off, cell_fan4_switch_off, cell_fan5_switch_off,
								cell_fan6_switch_off, cell_fan7_switch_off, cell_fan8_switch_off, cell_fan9_switch_off, cell_fan10_switch_off,
								cell_fan11_switch_off, cell_fan12_switch_off, cell_fan13_switch_off, cell_fan14_switch_off, cell_fan15_switch_off,
								cell_fan16_switch_off, cell_fan17_switch_off, cell_fan18_switch_off, cell_fan19_switch_off, cell_fan20_switch_off};

static sw_act node_set_high[]={node_id1_sw_on, node_id2_sw_on, node_id3_sw_on, node_id4_sw_on, node_id5_sw_on,
								node_id6_sw_on,	node_id7_sw_on,	node_id8_sw_on,	node_id9_sw_on,	node_id10_sw_on,
								node_id11_sw_on, node_id12_sw_on, node_id13_sw_on, node_id14_sw_on,	node_id15_sw_on,
								node_id16_sw_on, node_id17_sw_on, node_id18_sw_on, node_id19_sw_on,	node_id20_sw_on};

static sw_act node_set_low[]={node_id1_sw_off, node_id2_sw_off, node_id3_sw_off, node_id4_sw_off, node_id5_sw_off,
								node_id6_sw_off, node_id7_sw_off, node_id8_sw_off, node_id9_sw_off, node_id10_sw_off,
								node_id11_sw_off, node_id12_sw_off, node_id13_sw_off, node_id14_sw_off,	node_id15_sw_off,
								node_id16_sw_off, node_id17_sw_off, node_id18_sw_off, node_id19_sw_off,	node_id20_sw_off};


void peripheral_init(Cabinet_App* p_ca){
	for(uint8_t cab_id=0;cab_id<p_ca->bss.cab_num;cab_id++){
		p_ca->bss.cabs[cab_id].node_id_sw.sw_on = node_set_high[cab_id];
		p_ca->bss.cabs[cab_id].node_id_sw.sw_off = node_set_low[cab_id];
		p_ca->bss.cabs[cab_id].door.solenoid.sw_on = door_interface[cab_id];
		p_ca->bss.cabs[cab_id].door.io_state.get_io_state = ios_interface[cab_id];
		p_ca->bss.cabs[cab_id].cell_fan.sw_on = cell_fan_on_interface[cab_id];
		p_ca->bss.cabs[cab_id].cell_fan.sw_off = cell_fan_off_interface[cab_id];
		p_ca->bss.cabs[cab_id].data_serialize = cab_cell_data_serialze_impl;
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

	p_ca->base.rpdo_process = can_master_rpdo_process_impl;
	p_ca->slave_com = &rs485m;
}

static void ntc_init(Cabinet_App* p_ca){
	for(uint8_t cab_id = CAB1; cab_id < CAB15; cab_id++){
		p_ca->bss.cabs[cab_id].temp_ss->base.hw->adc_value = cab_temp[cab_id];
		ntc_set_lut(p_ca->bss.cabs[cab_id].temp_ss, ntc_lookups);
	}
}

static void rs485_set_tx_mode(RS485_Master* p_485m){
	(void)p_485m;
	HAL_MAX485_SET_DIR_TX;
}

static void rs485_set_rx_mode(RS485_Master* p_485m){
	(void)p_485m;
	HAL_MAX485_SET_DIR_RX;
}

/* ------------------------------------------------------------------------------ */

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
	if(p_hw->rx_data != '\0'){
		selex_bss_app.rx_data[selex_bss_app.rx_index] = p_hw->rx_data;
		selex_bss_app.rx_index++;
	}
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

static void can_master_rpdo_process_impl(const CAN_master* const p_cm){
	uint32_t cob_id = p_cm->p_hw->can_rx.StdId & 0xFFFFFF80;
    uint8_t node_id=(uint8_t)(p_cm->p_hw->can_rx.StdId & 0x7F);
    uint8_t cab_id=node_id - 5;
    if(bp_get_con_state(selex_bss_app.bss.cabs[cab_id].bp) != CO_SLAVE_CON_ST_CONNECTED){
    	return;
    }

    bp_reset_inactive_counter(selex_bss_app.bss.cabs[cab_id].bp);
    switch(cob_id){
    case BP_VOL_CUR_TPDO_COBID:
    	selex_bss_app.bss.cabs[cab_id].bp->vol=10*(uint32_t)CO_getUint16(p_cm->p_hw->rx_data);
    	selex_bss_app.bss.cabs[cab_id].bp->cur=(int32_t)10*((int16_t)CO_getUint16(p_cm->p_hw->rx_data+2));
    	selex_bss_app.bss.cabs[cab_id].bp->soc=p_cm->p_hw->rx_data[4];
    	selex_bss_app.bss.cabs[cab_id].bp->state=p_cm->p_hw->rx_data[5];
    	selex_bss_app.bss.cabs[cab_id].bp->status=(uint16_t)CO_getUint16(p_cm->p_hw->rx_data+6);
        break;
    case BP_LOW_CELLS_VOL_TPDO_COBID:
        CO_memcpy(selex_bss_app.bss.cabs[cab_id].bp->cell_vol,p_cm->p_hw->rx_data,8);
        break;
    case BP_HIGH_CELLS_VOL_TPDO_COBID:
        CO_memcpy(selex_bss_app.bss.cabs[cab_id].bp->cell_vol,p_cm->p_hw->rx_data,8);
        break;
    case BP_TEMP_TPDO_COBID:
        CO_memcpy((uint8_t*)selex_bss_app.bss.cabs[cab_id].bp->temp,p_cm->p_hw->rx_data,8);
        break;
    default:
    	break;
    }
}

static void cab_cell_data_serialze_impl(Cabinet* p_cc, char* buff){
	*buff++=':';
	*buff++='R';
	*buff++='C';
    *buff++=',';
	buff+=long_to_string(p_cc->cab_id,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->state,buff);
    *buff++=',';
    buff+=long_to_string(selex_bss_app.base.assign_state, buff);
    *buff++=',';
	buff+=long_to_string(p_cc->door.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->cell_fan.state,buff);
    *buff++=',';
	buff+=long_to_string(p_cc->temp,buff);
    *buff++=',';
    if(p_cc->bp->base.con_state==CO_SLAVE_CON_ST_CONNECTED){
        for(uint8_t i = 0; *(p_cc->bp->base.sn + i) != '\0'; i++){
        	*buff++= *(p_cc->bp->base.sn+i);
        }
    }
    else{
    	*buff++='0';
    }
    *buff++='*';
    *buff++='\n';
    *buff++='\0';
}

/* ------------------------------------------------------------------------------ */

static void node_id1_sw_on(Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id2_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id3_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id4_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id5_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id6_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id7_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id8_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id9_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id10_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id11_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id12_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id13_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id14_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id15_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id16_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id17_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id18_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id19_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id20_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_NODE_ID, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].node_id_sw.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void node_id1_sw_off(Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id2_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id3_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id4_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id5_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id6_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id7_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id8_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id9_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id10_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id11_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id12_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id13_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id14_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id15_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id16_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id17_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id18_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id19_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id20_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].node_id_sw.state = SW_ST_FAIL;
	}
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan2_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan3_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan4_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan5_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan6_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan7_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan8_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan9_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan10_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan11_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan12_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan13_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan14_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan15_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan16_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan17_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan18_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan19_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan20_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_FAN, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].cell_fan.state = SW_ST_FAIL;
	}
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan2_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan3_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan4_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan5_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan6_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan7_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan8_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan9_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan10_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan11_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan12_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan13_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan14_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan15_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan16_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan17_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan18_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan19_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].cell_fan.state = SW_ST_FAIL;
	}
}

static void cell_fan20_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_FAN, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].cell_fan.state = SW_ST_FAIL;
	}
}

/*--------------------------------------------------------------------------------*/

static DOOR_STATE door1_get_state( IO_State* p_io){
	(void) p_io;
	rs485_master_process_sync_data(&rs485m, 0);
	return selex_bss_app.bss.cabs[0].door.state;
}

static DOOR_STATE door2_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 1);
	return selex_bss_app.bss.cabs[1].door.state;
}

DOOR_STATE door3_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 2);
	return selex_bss_app.bss.cabs[2].door.state;
}

static DOOR_STATE door4_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 3);
	return selex_bss_app.bss.cabs[3].door.state;
}

static DOOR_STATE door5_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 4);
	return selex_bss_app.bss.cabs[4].door.state;
}

static DOOR_STATE door6_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 5);
	return selex_bss_app.bss.cabs[5].door.state;
}

static DOOR_STATE door7_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 6);
	return selex_bss_app.bss.cabs[6].door.state;
}

static DOOR_STATE door8_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 7);
	return selex_bss_app.bss.cabs[7].door.state;
}

static DOOR_STATE door9_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 8);
	return selex_bss_app.bss.cabs[8].door.state;
}

static DOOR_STATE door10_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 9);
	return selex_bss_app.bss.cabs[9].door.state;
}

static DOOR_STATE door11_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 10);
	return selex_bss_app.bss.cabs[10].door.state;
}

static DOOR_STATE door12_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 11);
	return selex_bss_app.bss.cabs[11].door.state;
}

static DOOR_STATE door13_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 12);
	return selex_bss_app.bss.cabs[12].door.state;
}

static DOOR_STATE door14_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 13);
	return selex_bss_app.bss.cabs[13].door.state;
}

static DOOR_STATE door15_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 14);
	return selex_bss_app.bss.cabs[14].door.state;
}

static DOOR_STATE door16_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 15);
	return selex_bss_app.bss.cabs[15].door.state;
}

static DOOR_STATE door17_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 16);
	return selex_bss_app.bss.cabs[16].door.state;
}

static DOOR_STATE door18_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 17);
	return selex_bss_app.bss.cabs[17].door.state;
}

static DOOR_STATE door19_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 18);
	return selex_bss_app.bss.cabs[18].door.state;
}

static DOOR_STATE door20_get_state( IO_State* p_io){
	(void)p_io;
	rs485_master_process_sync_data(&rs485m, 19);
	return selex_bss_app.bss.cabs[19].door.state;
}

/*--------------------------------------------------------------------------------*/

static void door1_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].door.state = SW_ST_FAIL;
	}
}

static void door2_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].door.state = SW_ST_FAIL;
	}
}

static void door3_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].door.state = SW_ST_FAIL;
	}
}

static void door4_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].door.state = SW_ST_FAIL;
	}
}

static void door5_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].door.state = SW_ST_FAIL;
	}
}

static void door6_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].door.state = SW_ST_FAIL;
	}
}

static void door7_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].door.state = SW_ST_FAIL;
	}
}

static void door8_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].door.state = SW_ST_FAIL;
	}
}

static void door9_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].door.state = SW_ST_FAIL;
	}
}

static void door10_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].door.state = SW_ST_FAIL;
	}
}

static void door11_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].door.state = SW_ST_FAIL;
	}
}

static void door12_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].door.state = SW_ST_FAIL;
	}
}

static void door13_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].door.state = SW_ST_FAIL;
	}
}

static void door14_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].door.state = SW_ST_FAIL;
	}
}

static void door15_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].door.state = SW_ST_FAIL;
	}
}

static void door16_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].door.state = SW_ST_FAIL;
	}
}

static void door17_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].door.state = SW_ST_FAIL;
	}
}

static void door18_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].door.state = SW_ST_FAIL;
	}
}

static void door19_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].door.state = SW_ST_FAIL;
	}
}

static void door20_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_DOOR, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].door.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void charger1_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].charger.state = SW_ST_FAIL;
	}
}

static void charger2_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].charger.state = SW_ST_FAIL;
	}
}

static void charger3_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].charger.state = SW_ST_FAIL;
	}
}

static void charger4_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].charger.state = SW_ST_FAIL;
	}
}

static void charger5_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].charger.state = SW_ST_FAIL;
	}
}

static void charger6_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].charger.state = SW_ST_FAIL;
	}
}

static void charger7_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].charger.state = SW_ST_FAIL;
	}
}

static void charger8_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].charger.state = SW_ST_FAIL;
	}
}

static void charger9_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].charger.state = SW_ST_FAIL;
	}
}

static void charger10_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].charger.state = SW_ST_FAIL;
	}
}

static void charger11_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].charger.state = SW_ST_FAIL;
	}
}

static void charger12_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].charger.state = SW_ST_FAIL;
	}
}

static void charger13_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].charger.state = SW_ST_FAIL;
	}
}

static void charger14_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].charger.state = SW_ST_FAIL;
	}
}

static void charger15_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].charger.state = SW_ST_FAIL;
	}
}

static void charger16_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].charger.state = SW_ST_FAIL;
	}
}

static void charger17_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].charger.state = SW_ST_FAIL;
	}
}

static void charger18_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].charger.state = SW_ST_FAIL;
	}
}

static void charger19_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].charger.state = SW_ST_FAIL;
	}
}

static void charger20_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_CHARGER, ACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].charger.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void charger1_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[0].charger.state = SW_ST_FAIL;
	}
}

static void charger2_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[1].charger.state = SW_ST_FAIL;
	}
}

static void charger3_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[2].charger.state = SW_ST_FAIL;
	}
}

static void charger4_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[3].charger.state = SW_ST_FAIL;
	}
}

static void charger5_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[4].charger.state = SW_ST_FAIL;
	}
}

static void charger6_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[5].charger.state = SW_ST_FAIL;
	}
}

static void charger7_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[6].charger.state = SW_ST_FAIL;
	}
}

static void charger8_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[7].charger.state = SW_ST_FAIL;
	}
}

static void charger9_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[8].charger.state = SW_ST_FAIL;
	}
}

static void charger10_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[9].charger.state = SW_ST_FAIL;
	}
}

static void charger11_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[10].charger.state = SW_ST_FAIL;
	}
}

static void charger12_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[11].charger.state = SW_ST_FAIL;
	}
}

static void charger13_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[12].charger.state = SW_ST_FAIL;
	}
}

static void charger14_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[13].charger.state = SW_ST_FAIL;
	}
}

static void charger15_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[14].charger.state = SW_ST_FAIL;
	}
}

static void charger16_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[15].charger.state = SW_ST_FAIL;
	}
}

static void charger17_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[16].charger.state = SW_ST_FAIL;
	}
}

static void charger18_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[17].charger.state = SW_ST_FAIL;
	}
}

static void charger19_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[18].charger.state = SW_ST_FAIL;
	}
}

static void charger20_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_CHARGER, DEACTIVE);
	if(rs485m->csv.state == FAIL){
		selex_bss_app.bss.cabs[19].charger.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void slave1_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_LED, ACTIVE);
}

static void slave2_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_LED, ACTIVE);
}

static void slave3_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_LED, ACTIVE);
}

static void slave4_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_LED, ACTIVE);
}

static void slave5_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_LED, ACTIVE);
}

static void slave6_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_LED, ACTIVE);
}

static void slave7_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_LED, ACTIVE);
}

static void slave8_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_LED, ACTIVE);
}

static void slave9_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_LED, ACTIVE);
}

static void slave10_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_LED, ACTIVE);
}

static void slave11_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_LED, ACTIVE);
}

static void slave12_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_LED, ACTIVE);
}

static void slave13_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_LED, ACTIVE);
}

static void slave14_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_LED, ACTIVE);
}

static void slave15_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_LED, ACTIVE);
}

static void slave16_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_LED, ACTIVE);
}

static void slave17_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_LED, ACTIVE);
}

static void slave18_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_LED, ACTIVE);
}

static void slave19_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_LED, ACTIVE);
}

static void slave20_led_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_LED, ACTIVE);
}

/* ------------------------------------------------------------------------------ */

static void slave1_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_LED, DEACTIVE);
}

static void slave2_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_LED, DEACTIVE);
}

static void slave3_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_LED, DEACTIVE);
}

static void slave4_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_LED, DEACTIVE);
}

static void slave5_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_LED, DEACTIVE);
}

static void slave6_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_LED, DEACTIVE);
}

static void slave7_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_LED, DEACTIVE);
}

static void slave8_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_LED, DEACTIVE);
}

static void slave9_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_LED, DEACTIVE);
}

static void slave10_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_LED, DEACTIVE);
}

static void slave11_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_LED, DEACTIVE);
}

static void slave12_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_LED, DEACTIVE);
}

static void slave13_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_LED, DEACTIVE);
}

static void slave14_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_LED, DEACTIVE);
}

static void slave15_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_LED, DEACTIVE);
}

static void slave16_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_LED, DEACTIVE);
}

static void slave17_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_LED, DEACTIVE);
}

static void slave18_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_LED, DEACTIVE);
}

static void slave19_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_LED, DEACTIVE);
}

static void slave20_led_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_LED, DEACTIVE);
}

/* ------------------------------------------------------------------------------ */
