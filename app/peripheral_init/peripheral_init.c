/*
 * peripheral_init.c
 *
 *  Created on: Apr 22, 2021
 *      Author: KhanhDinh
 */

#include "peripheral_init.h"
#include "cabinet_app.h"
#include "cabinet_hw_hal.h"

#if 0
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
#endif

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
static void cell_fan16_switch_off( Switch* p_sw);
static void cell_fan17_switch_off( Switch* p_sw);
static void cell_fan18_switch_off( Switch* p_sw);
static void cell_fan19_switch_off( Switch* p_sw);
static void cell_fan20_switch_off( Switch* p_sw);

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

static void led1_update_color(Cabinet_led* p_led);
static void led2_update_color(Cabinet_led* p_led);
static void led3_update_color(Cabinet_led* p_led);
static void led4_update_color(Cabinet_led* p_led);
static void led5_update_color(Cabinet_led* p_led);
static void led6_update_color(Cabinet_led* p_led);
static void led7_update_color(Cabinet_led* p_led);
static void led8_update_color(Cabinet_led* p_led);
static void led9_update_color(Cabinet_led* p_led);
static void led10_update_color(Cabinet_led* p_led);
static void led11_update_color(Cabinet_led* p_led);
static void led12_update_color(Cabinet_led* p_led);
static void led13_update_color(Cabinet_led* p_led);
static void led14_update_color(Cabinet_led* p_led);
static void led15_update_color(Cabinet_led* p_led);
static void led16_update_color(Cabinet_led* p_led);
static void led17_update_color(Cabinet_led* p_led);
static void led18_update_color(Cabinet_led* p_led);
static void led19_update_color(Cabinet_led* p_led);

static void bss_led_set_color_impl(BSS_Led* p_led);
static void bss_bp_backup_get_voltage_impl(BP_backup* p_bp_backup);
static void bss_siren_impl(void);
static void bss_charger_bp_backup_impl(BP_backup* p_bp_backup);
static void ntc_init(Cabinet_App* p_ca);
static void ntc_sensor_get_adc_value(NTC* p_ntc);

static void rs485_set_tx_mode(RS485_Master* p_485m);
static void rs485_set_rx_mode(RS485_Master* p_485m);

static void rs485_receive_handle_impl(UART_hw* p_hw);
static void hmi_receive_handle_impl(UART_hw* p_hw);
static void debug_receive_handle_impl(UART_hw* p_hw);

static void rs485_parse_slave_msg_handle_impl(RS485_Master* p_485m);

static void can_master_rpdo_process_impl(const CAN_master* const p_cm);

//static void cabinet_door_close_event_handle(Cabinet *p_cab);
//static void cabinet_door_open_event_handle(Cabinet *p_cab);
static void can_master_slave_select_impl(const CAN_master *p_cm, const uint32_t id);
static void can_master_slave_deselect_impl(const CAN_master *p_cm, const uint32_t id);
static void bp_assign_id_success_handle(const CAN_master *const p_cm, const uint32_t id);
static void bp_assign_id_fail_handle(CAN_master *p_cm, const uint32_t id);
static void reassign_attemp_handle(CAN_master* p_cm);

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

static sw_act node_id_high_interface[]={node_id1_sw_on, node_id2_sw_on, node_id3_sw_on, node_id4_sw_on, node_id5_sw_on,
								node_id6_sw_on,	node_id7_sw_on,	node_id8_sw_on,	node_id9_sw_on,	node_id10_sw_on,
								node_id11_sw_on, node_id12_sw_on, node_id13_sw_on, node_id14_sw_on,	node_id15_sw_on,
								node_id16_sw_on, node_id17_sw_on, node_id18_sw_on, node_id19_sw_on,	node_id20_sw_on};

static sw_act node_id_low_interface[]={node_id1_sw_off, node_id2_sw_off, node_id3_sw_off, node_id4_sw_off, node_id5_sw_off,
								node_id6_sw_off, node_id7_sw_off, node_id8_sw_off, node_id9_sw_off, node_id10_sw_off,
								node_id11_sw_off, node_id12_sw_off, node_id13_sw_off, node_id14_sw_off,	node_id15_sw_off,
								node_id16_sw_off, node_id17_sw_off, node_id18_sw_off, node_id19_sw_off,	node_id20_sw_off};

static sw_act charger_sw_on_interface[] = {charger1_sw_on, charger2_sw_on, charger3_sw_on, charger4_sw_on, charger5_sw_on,
								charger6_sw_on, charger7_sw_on, charger8_sw_on, charger9_sw_on, charger10_sw_on,
								charger11_sw_on, charger12_sw_on, charger13_sw_on, charger14_sw_on, charger15_sw_on,
								charger16_sw_on, charger17_sw_on, charger18_sw_on, charger19_sw_on, charger20_sw_on};

static sw_act charger_sw_off_interface[] = {charger1_sw_off, charger2_sw_off, charger3_sw_off, charger4_sw_off, charger5_sw_off,
								charger6_sw_off, charger7_sw_off, charger8_sw_off, charger9_sw_off, charger10_sw_off,
								charger11_sw_off, charger12_sw_off, charger13_sw_off, charger14_sw_off, charger15_sw_off,
								charger16_sw_off, charger17_sw_off, charger18_sw_off, charger19_sw_off, charger20_sw_off};

static led_act led_update_color[] = {led1_update_color, led2_update_color, led3_update_color, led4_update_color, led5_update_color,
								led6_update_color, led7_update_color, led8_update_color, led9_update_color, led10_update_color,
								led11_update_color, led12_update_color, led13_update_color, led14_update_color, led15_update_color,
								led16_update_color, led17_update_color, led18_update_color, led19_update_color};

void peripheral_init(Cabinet_App* p_ca){
	for(uint8_t cab_id = 0; cab_id < CABINET_CELL_NUM; cab_id++){
		p_ca->bss.cabs[cab_id].node_id_sw.sw_on 			= node_id_high_interface[cab_id];
		p_ca->bss.cabs[cab_id].node_id_sw.sw_off 			= node_id_low_interface[cab_id];
		p_ca->bss.cabs[cab_id].door.solenoid.sw_on 			= door_sw_interface[cab_id];
		p_ca->bss.cabs[cab_id].door.io_state.get_io_state 	= door_state_interface[cab_id];
		p_ca->bss.cabs[cab_id].cell_fan.sw_on 				= cell_fan_on_interface[cab_id];
		p_ca->bss.cabs[cab_id].cell_fan.sw_off 				= cell_fan_off_interface[cab_id];
		p_ca->bss.cabs[cab_id].charger.sw_on 				= charger_sw_on_interface[cab_id];
		p_ca->bss.cabs[cab_id].charger.sw_off 				= charger_sw_off_interface[cab_id];
		p_ca->bss.cabs[cab_id].led.update_color				= led_update_color[cab_id];
	}
	adc_hw_init();
	p_ca->bss.led.set_color = bss_led_set_color_impl;
	p_ca->bss.bp_backup.get_voltage = bss_bp_backup_get_voltage_impl;
	p_ca->slave_com = &rs485m;
	rs485_master_init(&rs485m, &cabinet_485_hw);
	rs485m.p_hw->uart_module = rs485_com.uart_module;
	rs485_com.receive_handle = rs485_receive_handle_impl;
	rs485m.set_transmit_mode = rs485_set_tx_mode;
	rs485m.set_receive_mode = rs485_set_rx_mode;
	rs485m.rx_index = 0;
	rs485m.state = RS485_MASTER_ST_IDLE;
	rs485m.parse_slave_msg_handle = rs485_parse_slave_msg_handle_impl;

	hmi_com.receive_handle = hmi_receive_handle_impl;
	debug_com.receive_handle = debug_receive_handle_impl;

	p_ca->base.rpdo_process = can_master_rpdo_process_impl;
	p_ca->base.slave_select = can_master_slave_select_impl;
	p_ca->base.slave_deselect = can_master_slave_deselect_impl;
	p_ca->base.on_slave_assign_fail = bp_assign_id_fail_handle;
	p_ca->base.on_slave_assign_success = bp_assign_id_success_handle;
	p_ca->base.reassign_attemp = reassign_attemp_handle;
}

#if 0
static void ntc_init(Cabinet_App* p_ca){
	for(uint8_t cab_id = CAB1; cab_id < CAB15; cab_id++){
		p_ca->bss.cabs[cab_id].temp_ss->base.hw->adc_value = cab_temp[cab_id];
		ntc_set_lut(p_ca->bss.cabs[cab_id].temp_ss, ntc_lookups);
	}
}
#endif

/* --------------------------------------------------------------------------------- */

static void can_master_slave_select_impl(const CAN_master *p_cm, const uint32_t id) {
	(void) p_cm;
	sw_off(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void can_master_slave_deselect_impl(const CAN_master *p_cm, const uint32_t id) {
	(void) p_cm;
	sw_on(&(selex_bss_app.bss.cabs[id].node_id_sw));
}

static void bp_assign_id_success_handle(const CAN_master *const p_cm, const uint32_t id) {
	(void) p_cm;
	sw_on(&(selex_bss_app.bss.cabs[id].node_id_sw));
	bp_reset_inactive_counter(selex_bss_app.bss.cabs[id].bp);
}

static void bp_assign_id_fail_handle(CAN_master *p_cm, const uint32_t id) {
	if(selex_bss_app.bss.state == BSS_ST_ACTIVE)
	{
		if(reassign_attemp_cnt < 4){
			p_cm->reassign_attemp(p_cm);
			return;
		}
		selex_bss_app.base.pdo_sync_timestamp = sys_timestamp + 100;
	}
	cab_cell_update_state(&selex_bss_app.bss.cabs[id]);
	sw_off(&(selex_bss_app.bss.cabs[id].node_id_sw));
    co_slave_set_con_state(p_cm->assigning_slave, CO_SLAVE_CON_ST_DISCONNECT);
	p_cm->assign_state = CM_ASSIGN_ST_DONE;
}

static void reassign_attemp_handle(CAN_master* p_cm){
	(void)p_cm;
	reassign_attemp_cnt++;
	can_master_start_assign_slave((CAN_master*)&selex_bss_app,
			selex_bss_app.base.assigning_slave,
			sys_timestamp);
}

/* --------------------------------------------------------------------------------- */

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
	if(p_hw->rx_data == 250){
		if(rs485m.rx_index == 0) p_hw->rx_data = ';';
		else p_hw->rx_data = '*';
	}
	if(p_hw->rx_data == '*'){
		rs485m.is_new_msg = 1;
	}
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
	if(p_hw->rx_data == '*'){
		selex_bss_app.is_new_msg = 1;
	}
	if(selex_bss_app.rx_index == 32){
		selex_bss_app.rx_index = 0;
		return;
	}
	if(p_hw->rx_data != '\0'){
		selex_bss_app.rx_data[selex_bss_app.rx_index] = p_hw->rx_data;
		selex_bss_app.rx_index++;
	}
}
static void debug_receive_handle_impl(UART_hw* p_hw){
#if 1
	if(p_hw->rx_data == '*'){
		selex_bss_app.is_new_msg = 1;
	}
	if(selex_bss_app.rx_index == 32){
		selex_bss_app.rx_index = 0;
		return;
	}
	if(p_hw->rx_data != '\0'){
		selex_bss_app.rx_data[selex_bss_app.rx_index] = p_hw->rx_data;
		selex_bss_app.rx_index++;
	}
#endif
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
			cab_cell_update_door_state(&selex_bss_app.bss.cabs[p_485m->csv.id], !string_to_long(token));
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
	uint8_t node_id = (uint8_t) (p_cm->p_hw->can_rx.StdId & 0x7F);
	uint8_t bp_id = node_id - p_cm->slave_start_node_id;

	if(bp_id >= p_cm->slave_num) return;
	//if(bp_get_con_state(selex_bss_app.bss.cabs[bp_id].bp) != (CO_SLAVE_CON_ST_CONNECTED && CO_SLAVE_CON_ST_AUTHORIZING))  return;
	bp_reset_inactive_counter(selex_bss_app.bss.cabs[bp_id].bp);

	switch(cob_id){
	case BP_VOL_CUR_TPDO_COBID:
//		selex_bss_app.bss.cabs[bp_id].bp->vol		= 10*(uint32_t)CO_getUint16(p_cm->p_hw->rx_data);
		if(10*(uint32_t)CO_getUint16(p_cm->p_hw->rx_data) < 70000 && 10*(uint32_t)CO_getUint16(p_cm->p_hw->rx_data) > 40000){
			selex_bss_app.bss.cabs[bp_id].bp->vol = 10*(uint32_t)CO_getUint16(p_cm->p_hw->rx_data);
		}
		if(10*((int16_t)CO_getUint16(p_cm->p_hw->rx_data + 2)) > -20000
				&& 10*((int16_t)CO_getUint16(p_cm->p_hw->rx_data + 2)) < 20000 ){
			selex_bss_app.bss.cabs[bp_id].bp->cur 		= 10*((int16_t)CO_getUint16(p_cm->p_hw->rx_data + 2));
		}
		if(1){
		selex_bss_app.bss.cabs[bp_id].bp->soc 		= p_cm->p_hw->rx_data[4];
		}
		selex_bss_app.bss.cabs[bp_id].bp->state 	= p_cm->p_hw->rx_data[5];
		selex_bss_app.bss.cabs[bp_id].bp->status	= CO_getUint16(p_cm->p_hw->rx_data + 6);
		selex_bss_app.bss.cabs[bp_id].bp->is_data_available++;
		break;
	case BP_LOW_CELLS_VOL_TPDO_COBID:
		CO_memcpy(selex_bss_app.bss.cabs[bp_id].bp->cell_vol, p_cm->p_hw->rx_data, 8);
		selex_bss_app.bss.cabs[bp_id].bp->is_data_available++;
		break;
	case BP_HIGH_CELLS_VOL_TPDO_COBID:
		CO_memcpy(selex_bss_app.bss.cabs[bp_id].bp->cell_vol + 8, p_cm->p_hw->rx_data, 8);
		selex_bss_app.bss.cabs[bp_id].bp->is_data_available++;
		break;
	case BP_TEMP_TPDO_COBID:
		CO_memcpy(selex_bss_app.bss.cabs[bp_id].bp->temp, p_cm->p_hw->rx_data, 8);
		for(uint8_t i = 0; i < 8; i++){
			if(selex_bss_app.bss.cabs[bp_id].bp->temp[i] > 110){
				selex_bss_app.bss.cabs[bp_id].bp->temp[i] = 0;
			}
		}
		selex_bss_app.bss.cabs[bp_id].bp->is_data_available++;
		break;
	default:
		break;
	}

	if(selex_bss_app.bss.cabs[bp_id].bp->is_data_available == 0){
		selex_bss_app.bss.cabs[bp_id].bp->is_data_available = 1;
	}
}

/* ------------------------------------------------------------------------------ */

static void node_id1_sw_on(Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[0].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id2_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[1].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id3_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[2].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id4_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[3].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id5_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[4].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id6_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[5].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id7_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[6].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id8_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[7].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id9_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[8].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id10_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[9].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id11_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[10].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id12_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[11].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id13_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[12].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id14_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[13].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id15_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[14].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id16_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[15].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id17_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[16].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id18_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[17].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id19_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[18].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id20_sw_on( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_NODE_ID, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[19].node_id_sw.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void node_id1_sw_off(Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[0].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id2_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[1].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id3_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[2].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id4_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[3].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id5_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[4].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id6_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[5].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id7_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[6].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id8_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[7].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id9_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[8].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id10_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[9].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id11_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[10].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id12_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[11].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id13_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[12].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id14_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[13].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id15_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[14].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id16_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[15].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id17_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[16].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id18_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[17].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id19_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[18].node_id_sw.state = SW_ST_FAIL;
	}
}

static void node_id20_sw_off( Switch* p_cm){
	(void)p_cm;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_NODE_ID, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[19].node_id_sw.state = SW_ST_FAIL;
	}
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan2_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan3_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan4_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan5_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan6_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan7_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan8_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan9_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan10_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan11_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan12_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan13_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan14_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan15_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan16_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan17_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan18_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan19_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan20_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_FAN, ACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

/*--------------------------------------------------------------------------------*/

static void cell_fan1_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan2_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan3_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan4_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan5_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan6_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan7_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan8_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan9_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan10_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan11_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan12_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan13_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan14_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan15_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan16_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan17_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan18_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan19_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
	}
}

static void cell_fan20_switch_off( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_FAN, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		p_sw->state = SW_ST_FAIL;
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
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[0], rs485m.csv.state);
	}
}

static void door2_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[1], rs485m.csv.state);
	}
}

static void door3_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[2], rs485m.csv.state);
	}
}

static void door4_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[3], rs485m.csv.state);
	}
}

static void door5_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[4], rs485m.csv.state);
	}
}

static void door6_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[5], rs485m.csv.state);
	}
}

static void door7_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[6], rs485m.csv.state);
	}
}

static void door8_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[7], rs485m.csv.state);
	}
}

static void door9_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[8], rs485m.csv.state);
	}
}

static void door10_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[9], rs485m.csv.state);
	}
}

static void door11_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[10], rs485m.csv.state);
	}
}

static void door12_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[11], rs485m.csv.state);
	}
}

static void door13_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[12], rs485m.csv.state);
	}
}

static void door14_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[13], rs485m.csv.state);
	}
}

static void door15_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[14], rs485m.csv.state);
	}
}

static void door16_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[15], rs485m.csv.state);
	}
}

static void door17_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[16], rs485m.csv.state);
	}
}

static void door18_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[17], rs485m.csv.state);
	}
}

static void door19_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[18], rs485m.csv.state);
	}
}

static void door20_switch_on( Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_DOOR, ACTIVE);
	if(rs485m.csv.state == FAIL){
		cab_cell_update_door_state(&selex_bss_app.bss.cabs[19], rs485m.csv.state);
	}
}

/* ------------------------------------------------------------------------------ */

static void charger1_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[0].charger.state = SW_ST_FAIL;
	}
}

static void charger2_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[1].charger.state = SW_ST_FAIL;
	}
}

static void charger3_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[2].charger.state = SW_ST_FAIL;
	}
}

static void charger4_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[3].charger.state = SW_ST_FAIL;
	}
}

static void charger5_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[4].charger.state = SW_ST_FAIL;
	}
}

static void charger6_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[5].charger.state = SW_ST_FAIL;
	}
}

static void charger7_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[6].charger.state = SW_ST_FAIL;
	}
}

static void charger8_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[7].charger.state = SW_ST_FAIL;
	}
}

static void charger9_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[8].charger.state = SW_ST_FAIL;
	}
}

static void charger10_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[9].charger.state = SW_ST_FAIL;
	}
}

static void charger11_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[10].charger.state = SW_ST_FAIL;
	}
}

static void charger12_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[11].charger.state = SW_ST_FAIL;
	}
}

static void charger13_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[12].charger.state = SW_ST_FAIL;
	}
}

static void charger14_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[13].charger.state = SW_ST_FAIL;
	}
}

static void charger15_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[14].charger.state = SW_ST_FAIL;
	}
}

static void charger16_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[15].charger.state = SW_ST_FAIL;
	}
}

static void charger17_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[16].charger.state = SW_ST_FAIL;
	}
}

static void charger18_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[17].charger.state = SW_ST_FAIL;
	}
}

static void charger19_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[18].charger.state = SW_ST_FAIL;
	}
}

static void charger20_sw_on(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_CHARGER, ACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[19].charger.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void charger1_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[0].charger.state = SW_ST_FAIL;
	}
}

static void charger2_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[1].charger.state = SW_ST_FAIL;
	}
}

static void charger3_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[2].charger.state = SW_ST_FAIL;
	}
}

static void charger4_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[3].charger.state = SW_ST_FAIL;
	}
}

static void charger5_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[4].charger.state = SW_ST_FAIL;
	}
}

static void charger6_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[5].charger.state = SW_ST_FAIL;
	}
}

static void charger7_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[6].charger.state = SW_ST_FAIL;
	}
}

static void charger8_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[7].charger.state = SW_ST_FAIL;
	}
}

static void charger9_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[8].charger.state = SW_ST_FAIL;
	}
}

static void charger10_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[9].charger.state = SW_ST_FAIL;
	}
}

static void charger11_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[10].charger.state = SW_ST_FAIL;
	}
}

static void charger12_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[11].charger.state = SW_ST_FAIL;
	}
}

static void charger13_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[12].charger.state = SW_ST_FAIL;
	}
}

static void charger14_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[13].charger.state = SW_ST_FAIL;
	}
}

static void charger15_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[14].charger.state = SW_ST_FAIL;
	}
}

static void charger16_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[15].charger.state = SW_ST_FAIL;
	}
}

static void charger17_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[16].charger.state = SW_ST_FAIL;
	}
}

static void charger18_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[17].charger.state = SW_ST_FAIL;
	}
}

static void charger19_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[18].charger.state = SW_ST_FAIL;
	}
}

static void charger20_sw_off(Switch* p_sw){
	(void)p_sw;
	rs485_master_process_switch_command(&rs485m, 19, SLAVE_CHARGER, DEACTIVE);
	if(rs485m.csv.state == FAIL){
		selex_bss_app.bss.cabs[19].charger.state = SW_ST_FAIL;
	}
}

/* ------------------------------------------------------------------------------ */

static void led1_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 0, SLAVE_LED, p_led->color);
}

static void led2_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 1, SLAVE_LED, p_led->color);
}

static void led3_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 2, SLAVE_LED, p_led->color);
}

static void led4_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 3, SLAVE_LED, p_led->color);
}

static void led5_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 4, SLAVE_LED, p_led->color);
}

static void led6_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 5, SLAVE_LED, p_led->color);
}

static void led7_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 6, SLAVE_LED, p_led->color);
}

static void led8_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 7, SLAVE_LED, p_led->color);
}

static void led9_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 8, SLAVE_LED, p_led->color);
}

static void led10_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 9, SLAVE_LED, p_led->color);
}

static void led11_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 10, SLAVE_LED, p_led->color);
}

static void led12_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 11, SLAVE_LED, p_led->color);
}

static void led13_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 12, SLAVE_LED, p_led->color);
}

static void led14_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 13, SLAVE_LED, p_led->color);
}

static void led15_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 14, SLAVE_LED, p_led->color);
}

static void led16_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 15, SLAVE_LED, p_led->color);
}

static void led17_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 16, SLAVE_LED, p_led->color);
}

static void led18_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 17, SLAVE_LED, p_led->color);
}

static void led19_update_color(Cabinet_led* p_led){
	rs485_master_process_switch_command(&rs485m, 18, SLAVE_LED, p_led->color);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void bss_led_set_color_impl(BSS_Led* p_led){
	BSS_LED_ENABLE;
	switch(p_led->color){
	case LED_NONE:
		BSS_DISABLE_LED;
		break;
	case LED_RED:
		BSS_CTRL_LED_RED_HIGH;
//		BSS_LED_BLUE_SET_LOW;
//		BSS_LED_RED_SET_HIGH;
		break;
	case LED_BLUE:
//		BSS_LED_RED_SET_LOW;
//		BSS_LED_GREEN_SET_LOW;
//		BSS_LED_BLUE_SET_HIGH;
		break;
	case LED_GREEN:
//		BSS_LED_BLUE_SET_LOW;
//		BSS_LED_RED_SET_LOW;
//		BSS_LED_GREEN_SET_HIGH;
		break;
	case LED_YELLOW:
//		BSS_LED_BLUE_SET_LOW;
//		BSS_LED_RED_SET_HIGH;
//		BSS_LED_GREEN_SET_HIGH;
		break;
	case LED_PINK:
//		BSS_LED_GREEN_SET_LOW;
//		BSS_LED_BLUE_SET_HIGH;
//		BSS_LED_RED_SET_HIGH;
		break;
	case LED_SKY:
//		BSS_LED_RED_SET_LOW;
//		BSS_LED_BLUE_SET_HIGH;
//		BSS_LED_GREEN_SET_LOW;
		break;
	case LED_WHITE:
//		BSS_LED_RED_SET_HIGH;
//		BSS_LED_BLUE_SET_HIGH;
//		BSS_LED_GREEN_SET_LOW;
		break;
	default:
		break;
	}
}
static void bss_siren_impl(void)
{
	BSS_CTRL_SPEAKER_HIGH;
}

static void bss_charger_bp_backup_impl(BP_backup* p_bp_backup)
{
	if(p_bp_backup->is_charger)
	{
		CHG_ENB_LOW;
	}
	else
	{
		CHG_ENB_LOW;
	}
}

static void bss_bp_backup_get_voltage_impl(BP_backup* p_bp_backup)
{
	HAL_ADC_Start(&pin_vol.adc_module);
	p_bp_backup->vol = 3.61*VREF*(pin_vol.adc_value - pin_vol.adc_offset)/ADC_RESOLUTION;
	HAL_ADC_Stop(&pin_vol.adc_module);
}
