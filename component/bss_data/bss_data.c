/*
 * bss_data.c
 *
 *  Created on: Apr 18, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_BSS_DATA_BSS_DATA_C_
#define COMPONENT_BSS_DATA_BSS_DATA_C_

#include "bss_data.h"

static uint8_t		id_assign_cabs_charger[CHARGER_NUM][MAX_ASSIGNED_CABINET]
					= {{0,1,4,5,8,9,12,13,16}, {2,3,6,7,10,11,14,15,17,18}};

//static uint8_t		id_assign_cabs_charger[CHARGER_NUM][MAX_ASSIGNED_CABINET]					//0123
//					= {{0,3}, {1,2,4,5}};


static void bss_update_io_state(BSS_Data* p_bss);
static void bss_data_serialize_impl(BSS_Data* p_bss, char* buff);
static void all_data_cabinet_serialize_impl(BSS_Data* p_bss,char* buff);
void bss_init(BSS_Data* p_bss){
	p_bss->state = BSS_ST_INIT;
	p_bss->data_serialize = bss_data_serialize_impl;
	p_bss->all_data_cabinet_serialize = all_data_cabinet_serialize_impl;
	p_bss->bss_temps[0] = p_bss->bss_temps[1] = 30;

	p_bss->ac_chargers[1].assigned_cab_num = MAX_ASSIGNED_CAB_NUM;
	if(p_bss->cab_num%2 == 0){
		p_bss->ac_chargers[0].assigned_cab_num = MAX_ASSIGNED_CAB_NUM;
	}
	else p_bss->ac_chargers[0].assigned_cab_num = MAX_ASSIGNED_CAB_NUM - 1;
}

void bss_charger_init(BSS_Data* p_bss){
	for(uint8_t i = 0; i < p_bss->charger_num; i++){
		p_bss->ac_chargers[i].charging_cabin = NULL;
		p_bss->ac_chargers[i].input_power.state = SW_ST_OFF;
		for(uint8_t j = 0; j < p_bss->ac_chargers[i].assigned_cab_num; j++){
			uint8_t id = id_assign_cabs_charger[i][j];
			p_bss->ac_chargers[i].assigned_cabs[j] = &p_bss->cabs[id];
		}
	}
}

void bss_set_state(BSS_Data* p_bss, BSS_STATE new_state){
	BSS_STATE old_state = p_bss->state;
	if(old_state != new_state){
		p_bss->state = new_state;
		p_bss->is_changed = 1;
	}

	switch(p_bss->state){
	case BSS_ST_MAINTAIN:
		if(old_state == BSS_ST_ACTIVE){
			for(uint8_t i = 0; i < 2; i++){
				p_bss->ac_chargers[i].charging_cabin = NULL;
			}

			for(uint8_t i = 0; i < p_bss->cab_num; i++){
				if(p_bss->cabs[i].op_state != CAB_CELL_ST_INACTIVE){
					cab_cell_reset_io(&p_bss->cabs[i]);
					p_bss->cabs[i].op_state = CAB_CELL_ST_INIT;
					p_bss->cabs[i].is_changed = 1;
				}
			}
		}

		break;
	case BSS_ST_ACTIVE:
		for(uint8_t i = 0; i < p_bss->cab_num; i++){
			p_bss->cabs[i].is_changed = 1;
			cab_cell_update_led_state(&p_bss->cabs[i]);
		}
		bss_set_led_color(p_bss, LED_PINK);
		break;
	case BSS_ST_FAIL:
		break;
	case BSS_ST_UPGRADE_FW_BP:
		break;
	default:
		break;
	}
}

void bss_update_cabinets_state(BSS_Data* p_bss){
	for(uint8_t i = 0; i < p_bss->cab_num; i++){
		if(p_bss->cabs[i].op_state == CAB_CELL_ST_INACTIVE) continue;
		cab_cell_update_state(&p_bss->cabs[i]);
	}
}

void bss_update_ac_meter(BSS_Data* p_bss){


}

void bss_warning(BSS_Data* p_bss)
{
//	bss_siren();

}

static void bss_update_io_state(BSS_Data* p_bss){

	for(uint8_t i=0;i<p_bss->cab_num;i++){
		if(p_bss->cabs[i].op_state == CAB_CELL_ST_INACTIVE) continue;
		cab_cell_update_door_state(&p_bss->cabs[i], (DOOR_STATE)io_get_state(&p_bss->cabs[i].door.io_state));
	}
}

Cabinet* bss_get_cab_need_charge(BSS_Data* p_bss, uint8_t charger_id){
	Cabinet* cab = &p_bss->cabs[p_bss->cab_num];
	uint8_t available_cab_num = 0;		/* Available Cabinet Number */

	/* Get BP which has highest voltage to charge first */
	for(uint8_t i = 0; i < p_bss->ac_chargers[charger_id].assigned_cab_num; i++){
		if((p_bss->ac_chargers[charger_id].assigned_cabs[i]->bp->vol > 0)
				&& (p_bss->ac_chargers[charger_id].assigned_cabs[i]->bp->vol <= BP_START_CHARGE_THRESHOLD)
				&& (p_bss->ac_chargers[charger_id].assigned_cabs[i]->bp->state != BP_ST_DISCHARGING)
				&& (p_bss->ac_chargers[charger_id].assigned_cabs[i]->bp->temp[1] < BP_TEMP_START_CHARGER)
				&& (p_bss->ac_chargers[charger_id].assigned_cabs[i]->bp->base.con_state == CO_SLAVE_CON_ST_CONNECTED)){
			available_cab_num++;
			if(cab->bp->vol < p_bss->ac_chargers[charger_id].assigned_cabs[i]->bp->vol){
				cab = p_bss->ac_chargers[charger_id].assigned_cabs[i];
			}
		}
	}
	if(available_cab_num == 0) return NULL;
	else return cab;
}
// :R,S,0,A*
// --> R,S,0,A,19,0,0,[0,0],[0,0],[0,0,0,0],[30,30]
static void bss_data_serialize_impl(BSS_Data* p_bss, char* buff){
	*buff++=':';
	*buff++='R';
    *buff++=',';
	*buff++='S';
    *buff++=',';
    *buff++='0';
    *buff++=',';
    *buff++='A';
    *buff++=',';
	buff+=long_to_string(p_bss->cab_num,buff);
    *buff++=',';
	buff+=long_to_string(p_bss->state,buff);
    *buff++=',';
	buff+=long_to_string(p_bss->tilt_ss.state,buff);
	*buff++=',';
	// AC meter
	*buff++='[';
	buff+=long_to_string(p_bss->ac_meter.ac_voltage,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->ac_meter.ac_current,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->ac_meter.ac_power,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->ac_meter.cos,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->ac_meter.freq,buff);
	*buff++=',';
	buff+=long_to_string(p_bss->ac_meter.total_power,buff);
	*buff++=']';
	*buff++=',';
    // Chargers
    *buff++='[';
    for(uint8_t i = 0; i < 2; i++){
    	buff+=long_to_string((uint32_t)p_bss->ac_chargers[i].charging_cabin, buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++=',';
    // BSS_Fans
    *buff++='[';
    for(uint8_t i = 0; i < 2; i++){
    	buff+=long_to_string(p_bss->bss_fans[i].state, buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++=',';
    // BSS_Lamps
    *buff++='[';
    for(uint8_t i = 0; i < 4; i++){
    	buff+=long_to_string(p_bss->bss_lamps[i].state, buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++=',';
    // BSS_temps
    *buff++='[';
    for(uint8_t i = 0; i < 2; i++){
    	buff+=long_to_string(p_bss->bss_temps[i], buff);
    	*buff++=',';
    }
    --buff;
    *buff++=']';
    *buff++='*';
    *buff++='\0';
    *buff++='\n';
}
static void all_data_cabinet_serialize_impl(BSS_Data* p_bss,char* buff){
	*buff++ = ':';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[0].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[0].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[0].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[0].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[0].temp, buff);

	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[1].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[1].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[1].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[1].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[1].temp, buff);

	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[2].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[2].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[2].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[2].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[2].temp, buff);

	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[3].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[3].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[3].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[3].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[3].temp, buff);

	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[4].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[4].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[4].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[4].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[4].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[5].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[5].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[5].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[5].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[5].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[6].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[6].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[6].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[6].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[6].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[7].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[7].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[7].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[7].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[7].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[8].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[8].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[8].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[8].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[8].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[9].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[9].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[9].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[9].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[9].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[10].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[10].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[10].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[10].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[10].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[11].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[11].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[11].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[11].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[11].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[12].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[12].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[12].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[12].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[12].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[13].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[13].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[13].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[13].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[13].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[14].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[14].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[14].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[14].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[14].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[15].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[15].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[15].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[15].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[15].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[16].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[16].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[16].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[16].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[16].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[17].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[17].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[17].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[17].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[17].temp, buff);
	*buff++ = ',';
	*buff++ = 'R';
	*buff++ = ',';
	*buff++ = 'C';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[18].cab_id, buff);
	*buff++ = ',';
	*buff++ = 'A';
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[18].op_state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[18].door.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[18].cell_fan.state, buff);
	*buff++ = ',';
	buff += long_to_string(p_bss->cabs[18].temp, buff);

	*buff++ = '*';
	*buff++ = '\n';
	*buff++ = '\0';
}
#endif /* COMPONENT_BSS_DATA_BSS_DATA_C_ */
