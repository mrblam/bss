/*
 * app_co_init.c
 *
 *  Created on: Jun 17, 2022
 *      Author: Dakaka
 */

#include "app_co_init.h"
#include <stdbool.h>

#if 0
/* Save all communication profile data value in flash to use after reset */
static void app_co_save_all_comm_data_in_flash(void){}
/* Save all manufacturer-specific profile data value in flash to use after reset */
static void app_co_save_all_app_data_in_flash(void){}

/* Update all communication profile initializer data value from flash */
static void app_co_update_all_init_comm_data_from_flash(void){}
/* Update all manufacturer-specific profile initializer data value from flash */
static void app_co_update_all_init_app_data_from_flash(void){}
#endif

/* Declare for BS_TPDO1 */
static void bs_tpdo1_init(BS_TPDO1* p_bs_tpdo1);
static inline void bs_tpdo1_process(BS_TPDO1 *p_bs_tpdo1);
static void bs_tpdo1_dri_update_mode_command(BS_TPDO1* p_bs_tpdo1);
BS_TPDO1 bs_tpdo1;

static OPTIMIZE_ATTR void smc_error_handle(void);


/********************************************************************************************
 * 									MANDATORY: Edit carefully !!!
 * *******************************************************************************************/
/* Declare for build TPDO message function when using "tpdo_build_data_manually" mode*/
static void tpdo1_build_data_impl(uint8_t* buffer);
static void tpdo2_build_data_impl(uint8_t* buffer);
static void tpdo3_build_data_impl(uint8_t* buffer);
static void tpdo4_build_data_impl(uint8_t* buffer);

void* tpdo_build_data_impl[TPDO_NUMBER] =
{
		tpdo1_build_data_impl,
		tpdo2_build_data_impl,
		tpdo3_build_data_impl,
		tpdo4_build_data_impl
};

static void tpdo1_build_data_impl(uint8_t* buffer)
{
	co_seg_uint32_t data;
	data.w = 0;
	/* Add speed_rpm/acc_limit mode */
	data.w = MC_get_speed_rpm(&mc);
	buffer[7] = data.byte1.w;
	buffer[6] = data.byte0.w;

	/* Add throttle command */
	data.w = mc.torque_max * throttle_get_setpoint(&mc.throttle);
	buffer[5] = (uint8_t) (data.w);

	/* Add Estimated torque */
	buffer[4] = (uint8_t) MC_MAINTNC_get_est_torque_Nm(&mc);

	/* Add Estimated DC current */
	buffer[3] = mc.ibat_estimate;

	/* Add Allowed DC current */
	buffer[2] = mc.ibat_limit;

	/* Add selex error code */
	buffer[1] = MC_get_selex_error_code_sending(&mc);

	/**/
	data.byte0.b7 = MC_get_brake_state(&mc);
	data.byte0.b6 = anti_theft_get_activate_status(&mc.anti_theft) >> 1;
	data.byte0.b5 = anti_theft_get_activate_status(&mc.anti_theft);
	data.byte0.b4 = 0;
	buffer[0] = data.byte0.w;
}

static void tpdo2_build_data_impl(uint8_t* buffer)
{
	co_seg_uint32_t data;
	data.w = 0;
	/* Add revolution counter */
	data.w = mc.change_hall_position_counter;
	buffer[7] = data.byte3.w;
	buffer[6] = data.byte2.w;
	buffer[5] = data.byte1.w;
	buffer[4] = data.byte0.w;

	/**/
	buffer[3] = (uint8_t) temp_get_temperature(mc.p_board_temp);

	/**/
	buffer[2] = (uint8_t) temp_get_temperature(mc.p_motor_temp);

	/**/
	buffer[1] = MC_get_status(&mc);

}

static void tpdo3_build_data_impl(uint8_t* buffer)
{
	co_seg_uint32_t data;
	data.w = 0;
	/* Add dc bus voltage _mV */
	//data = p_mc->vdc_input * 1000;
	data.w = MC_MAINTNC_get_avg_eff_km_per_kWh(&mc);
	buffer[7] = data.byte3.w;
	buffer[6] = data.byte2.w;
	buffer[5] = data.byte1.w;
	buffer[4] = data.byte0.w;

}

static void tpdo4_build_data_impl(uint8_t* buffer)
{
}

/* Define can_send message function */
static void app_co_can_send_impl(CO_CAN_Msg* p_msg)
{
	can_hardware_send_message(p_can_hw, (p_msg->data_len << 16), p_msg->id.can_id, p_msg->data);
}
/* Call in receive can interrupt */
void app_co_can_receive(const uint32_t can_id, uint8_t* data)
{
	if(CO_can_receive_basic_handle(&CO_DEVICE, can_id, data)) ;
	/* USER CODE BEGIN */
	else if(can_id == CO_CAN_ID_PMU_TPDO_1)
	{
		CO_memcpy(bs_tpdo1.rx_msg.data, data, 8);
		bs_tpdo1.rx_msg.is_new = true;
	}
	else if(can_id == CO_CAN_ID_PMU_TPDO_2)
	{
		uint16_t temp = CO_getUint16(&data[6]);
		MC_MAINTNC_set_PMU_DCbus_voltage_V(&mc, temp*0.01f);

		temp = CO_getUint16(&data[4]);
		MC_MAINTNC_set_PMU_DCbus_current_A(&mc, temp*0.01f);
	}
	/* USER CODE END */
}

void app_co_init(void)
{
	app_co_init_storage();
	/* [Mandatory] Set CO_CAN_send interface */
	CO_CAN_set_can_send_interface(app_co_can_send_impl);

	/* [Mandatory] Init CO object */
	CO_init_basic(&CO_DEVICE,
			od_comm_prof_init_data.x1000_device_type,
			&od_comm_prof_init_data.x1018_identity,
			p_co_od);

	CO_SYNC_init(&CO_DEVICE.sync, &od_comm_prof_init_data);

	CO_TPDO_init(&CO_DEVICE.tpdos[0],
			&CO_DEVICE.sync,
			&od_comm_prof_init_data.x1A0x_tpdo_map_para[0],
			CO_DEVICE.p_od,
			&od_comm_prof_init_data.x180x_tpdo_comm_para[0],
			CO_TPDO_build_data_map, tpdo_build_data_impl[0]);

	for(uint8_t i =1; i < TPDO_NUMBER; i++)
	{
		CO_TPDO_init(&CO_DEVICE.tpdos[i],
				&CO_DEVICE.sync,
				&od_comm_prof_init_data.x1A0x_tpdo_map_para[i],
				CO_DEVICE.p_od,
				&od_comm_prof_init_data.x180x_tpdo_comm_para[i],
				CO_TPDO_build_data_manually, tpdo_build_data_impl[i]);
	}
	CO_SDOserver_init(&CO_DEVICE.sdo_server,
			&od_comm_prof_init_data,
			&CO_DEVICE.sync,
			CO_DEVICE.p_od);
	CO_SDOclient_init(&CO_DEVICE.sdo_client,
			&od_comm_prof_init_data,
			&CO_DEVICE.sync);


	/* USER CODE BEGIN */
	CO_EMCY_set_error_handle(&CO_DEVICE.emcy, smc_error_handle);
	bs_tpdo1_init(&bs_tpdo1);
	/* USER CODE END */
}


/********************************************************************************************
 * 									USER CODE
 * *******************************************************************************************/

void co_process_rewrite(CO* p_co, uint16_t time_diff_ms)
{
	bool sync_was;
	sync_was = CO_SYNC_process(&p_co->sync, 1, time_diff_ms);

	CO_SDOserver_process(&p_co->sdo_server, time_diff_ms);
	CO_SDOclient_process(&p_co->sdo_client, time_diff_ms);

    for (uint8_t i = 0; i < 3; i++)
    {
        CO_TPDO_process(&p_co->tpdos[i],time_diff_ms, sync_was);
    }

	if(&bs_tpdo1.rx_msg.is_new)
	{
		bs_tpdo1_process(&bs_tpdo1);
		bs_tpdo1.rx_msg.is_new = false;
	}
}

static OPTIMIZE_ATTR void smc_error_handle(void)
{
	uint32_t error_code = MC_get_error(&mc);
	switch(error_code){
	case SMC_ERR_NONE:
		break;
	case SMC_ERR_OC:
		MC_set_fault_state(&mc, FAULT_BIT_OVER_CURRENT);
		break;
	case SMC_ERR_HALL_ANGLE_STARTING:
	case SMC_ERR_HALL_ANGLE_RUNNING:
	case SMC_ERR_HALL_SEQ_INVALID:
	case SMC_ERR_HALL_INIT:
	case SMC_ERR_HALL_VIBRATION:
	case SMC_ERR_HALL_CABLE1:
	case SMC_ERR_HALL_CABLE2:
	case SMC_ERR_HALL_CABLE3:

		MC_set_fault_state(&mc, FAULT_BIT_HALL);
		break;
	case SMC_ERR_OV:
	case SMC_ERR_UV:
		MC_set_fault_state(&mc, FAULT_BIT_UV_OV);
		break;
	case SMC_ERR_OVER_LOAD:
		mc.fault_stall_recover_cter = 0;
		MC_set_fault_state(&mc, FAULT_BIT_OVER_LOAD);
		break;
	case SMC_ERR_THROTTLE_TIMEOUT:
	case SMC_ERR_THROTTLE_OUT_RANGE:
	case SMC_ERR_THROTTLE_NOT_RELEASE:
		MC_set_fault_state(&mc, FAULT_BIT_THROTTLE);
		break;
	case SMC_ERR_BOARD_OVERHEAT_LV2:
		MC_set_fault_state(&mc, FAULT_BIT_BOARD_OVERHEATING_LV2);
		break;
	case SMC_ERR_MOTOR_OVERHEAT_LV2:
		MC_set_fault_state(&mc, FAULT_BIT_MOTOR_OVERHEATING_LV2);
		break;
	default:
		//motor_set_fault_state_(p_mc, FAULT_BIT_DEFAULT);
		break;
	}

}

/*** BS_TPDO1 */
static void bs_tpdo1_init(BS_TPDO1* p_bs_tpdo1)
{
	p_bs_tpdo1->limited_discharge_cur_A = 35;
	p_bs_tpdo1->limited_charge_cur_A = 5;
  	p_bs_tpdo1->dri_mode_cm.mode	= PDO_DRI_SPORT;
  	p_bs_tpdo1->dri_mode_cm.update_status = PDO_DRI_UPDATED;

	for( int index = 0; index <=7 ; index++)
	{
		p_bs_tpdo1->rx_msg.data[index] = 0;
	}
	p_bs_tpdo1->rx_msg.is_new = 0;
}

static inline void bs_tpdo1_process(BS_TPDO1 *p_bs_tpdo1)
{
	uint16_t temp = CO_getUint16(&p_bs_tpdo1->rx_msg.data[7]);
	p_bs_tpdo1->limited_charge_cur_A = ((float)temp * 0.01f);
	temp = CO_getUint16(&p_bs_tpdo1->rx_msg.data[4]);
	p_bs_tpdo1->limited_discharge_cur_A = ((float)temp * 0.01f);

	bs_tpdo1_dri_update_mode_command(p_bs_tpdo1);

	co_seg_uint8_t temp1;
	temp1.w = p_bs_tpdo1->rx_msg.data[0];
	mc.parking_st = temp1.b5;
}
static void bs_tpdo1_dri_update_mode_command(BS_TPDO1* p_bs_tpdo1)
{
	PDO_DRIVE_MODE pre_mode = p_bs_tpdo1->dri_mode_cm.mode;
	/* Update current mode command */
	uint8_t eco_is_active = p_bs_tpdo1->rx_msg.data[0] & BS_TPDO_1_BYTE0_BIT4_ECO_SPORT;
	if(eco_is_active)
	{
		p_bs_tpdo1->dri_mode_cm.mode = PDO_DRI_ECO;
	}
	else
	{
		p_bs_tpdo1->dri_mode_cm.mode = PDO_DRI_SPORT;
	}
	/* Change driving mode command update status */
	if(p_bs_tpdo1->dri_mode_cm.mode != pre_mode)
	{
		p_bs_tpdo1->dri_mode_cm.update_status = PDO_DRI_NOT_UPDATE;
	}

}
