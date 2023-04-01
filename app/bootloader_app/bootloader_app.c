/*
 * bootloader_app.c
 *
 *  Created on: Mar 30, 2023
 *      Author: hoanpx
 */

#include "bootloader_app.h"

static sm_fw_signature_t g_new_fw_signature = {
		.m_size = 0,
		.m_crc = 0,
		.m_addr = 0,
		.m_is_valid = 0,
		.m_version = {0x01, 0x00, 0x02}
};

static int32_t g_total_seg = 0;
static int32_t g_current_seg = 0;
sm_segment_t segment_test;

void get_fw_info(sm_fw_signature_t* signature,Cabinet_App* p_ca){ // get infomation all data download to Bp
//	signature->m_crc = CRC_CalculateCRC16(&p_ca->hmi_csv.firmware[0], 1024);
	signature->m_addr = 0;
	signature->m_is_valid = 0;
	signature->m_size = 0;
	signature->m_version[0] = 0;
	signature->m_version[1] = 0;
}

void get_seg_fw(sm_segment_t* segment,Cabinet_App* p_ca){ // get infomation of 1 segment
	segment->m_crc = CRC_CalculateCRC16(&p_ca->hmi_csv.firmware[0], 2);
	segment->m_addr = 0;
	segment->m_index = 0;
	segment->m_data[2] = 0;
	segment->m_length = 0;
	segment->m_size = 0;
	segment->m_is_last = 0;

}
int32_t sm_seg_fill_data(sm_segment_t* _self, const uint8_t* data, int32_t len){//FILL data to m_data
    uint8_t segmentl;
//    g_toltal_seg = 0;
//    g_curret_segment;
}

