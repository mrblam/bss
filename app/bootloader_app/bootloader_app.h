/*
 * bootloader_app.h
 *
 *  Created on: Mar 30, 2023
 *      Author: hoanpx
 */

#ifndef APP_BOOTLOADER_APP_BOOTLOADER_APP_H_
#define APP_BOOTLOADER_APP_BOOTLOADER_APP_H_

#include "stm32f1xx_hal.h"
#include "cabinet_app.h"
#include "crc.h"
#define SM_FW_VERSION_LENGTH	2
#define SM_SEGMENT_FW_SIZE		2

typedef struct sm_fw_signature{
	uint8_t		m_version[SM_FW_VERSION_LENGTH];
	uint32_t	m_addr;
	uint32_t	m_size;
	uint16_t	m_crc;
	uint8_t		m_is_valid;
}sm_fw_signature_t;

typedef struct sm_segment{
	int32_t		m_addr;
	int32_t		m_index;
	int32_t		m_size;
	int32_t		m_length;
	uint8_t		m_data[SM_SEGMENT_FW_SIZE];
	uint16_t	m_crc;
	uint8_t		m_is_last;
}sm_segment_t;

extern sm_segment_t segment_test;
extern sm_fw_signature_t			fw_signature;
extern sm_segment_t					segment;

void get_fw_info(sm_fw_signature_t* signature, Cabinet_App* p_ca);
void get_seg_fw(sm_segment_t* segment,Cabinet_App* p_ca);

#endif /* APP_BOOTLOADER_APP_BOOTLOADER_APP_H_ */
