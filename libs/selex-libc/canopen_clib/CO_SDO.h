#ifndef CO_SDO_H_
#define CO_SDO_H_

#include "stdint.h"
#include "stdbool.h"

#include "../../../libs/selex-libc/canopen_clib/CAN_Msg.h"
#include "../../../libs/selex-libc/canopen_clib/CO_OD.h"

typedef struct CO_SDO_t CO_SDO;
/**
 * Internal states of the SDO server state machine
 */
typedef enum {
    CO_SDO_ST_IDLE                  = 0x00U,
    CO_SDO_ST_DOWNLOAD_INITIATE     = 0x11U,
    CO_SDO_ST_DOWNLOAD_SEGMENTED    = 0x12U,
    CO_SDO_ST_DOWNLOAD_BL_INITIATE  = 0x14U,
    CO_SDO_ST_DOWNLOAD_BL_SUBBLOCK  = 0x15U,
    CO_SDO_ST_DOWNLOAD_BL_SUB_RESP  = 0x16U,
    CO_SDO_ST_DOWNLOAD_BL_END       = 0x17U,
    CO_SDO_ST_UPLOAD_INITIATE       = 0x21U,
    CO_SDO_ST_UPLOAD_SEGMENTED      = 0x22U,
    CO_SDO_ST_UPLOAD_BL_INITIATE    = 0x24U,
    CO_SDO_ST_UPLOAD_BL_INITIATE_2  = 0x25U,
    CO_SDO_ST_UPLOAD_BL_SUBBLOCK    = 0x26U,
    CO_SDO_ST_UPLOAD_BL_END         = 0x27U
} CO_SDO_state_t;

/* Client command specifier */
#define CCS_DOWNLOAD_INITIATE           1
#define CCS_DOWNLOAD_SEGMENT            0

#define CCS_UPLOAD_INITIATE             2
#define CCS_UPLOAD_SEGMENT              3

#define CCS_ABORT                       4

#define CCS_UPLOAD_BLOCK                5
#define CCS_DOWNLOAD_BLOCK              6

/* Server Command Specifier */
#define SCS_UPLOAD_INITIATE             2
#define SCS_UPLOAD_SEGMENT              0

#define SCS_DOWNLOAD_INITIATED          3
#define SCS_DOWNLOAD_SEGMENT            1

#define SCS_ABORT                       4

#define SCS_DOWNLOAD_BLOCK              5
#define SCS_UPLOAD_BLOCK                6

/**
 * SDO abort codes.
 *
 * Send with Abort SDO transfer message.
 *
 * The abort codes not listed here are reserved.
 */
typedef enum{
    CO_SDO_AB_NONE                  = 0x00000000UL, /**< 0x00000000, No abort */
    CO_SDO_AB_TOGGLE_BIT            = 0x05030000UL, /**< 0x05030000, Toggle bit not altered */
    CO_SDO_AB_TIMEOUT               = 0x05040000UL, /**< 0x05040000, SDO protocol timed out */
    CO_SDO_AB_CMD                   = 0x05040001UL, /**< 0x05040001, Command specifier not valid or unknown */
    CO_SDO_AB_BLOCK_SIZE            = 0x05040002UL, /**< 0x05040002, Invalid block size in block mode */
    CO_SDO_AB_SEQ_NUM               = 0x05040003UL, /**< 0x05040003, Invalid sequence number in block mode */
    CO_SDO_AB_CRC                   = 0x05040004UL, /**< 0x05040004, CRC error (block mode only) */
    CO_SDO_AB_OUT_OF_MEM            = 0x05040005UL, /**< 0x05040005, Out of memory */
    CO_SDO_AB_UNSUPPORTED_ACCESS    = 0x06010000UL, /**< 0x06010000, Unsupported access to an object */
    CO_SDO_AB_WRITEONLY             = 0x06010001UL, /**< 0x06010001, Attempt to read a write only object */
    CO_SDO_AB_READONLY              = 0x06010002UL, /**< 0x06010002, Attempt to write a read only object */
    CO_SDO_AB_NOT_EXIST             = 0x06020000UL, /**< 0x06020000, Object does not exist */
    CO_SDO_AB_NO_MAP                = 0x06040041UL, /**< 0x06040041, Object cannot be mapped to the PDO */
    CO_SDO_AB_MAP_LEN               = 0x06040042UL, /**< 0x06040042, Number and length of object to be mapped exceeds PDO length */
    CO_SDO_AB_PRAM_INCOMPAT         = 0x06040043UL, /**< 0x06040043, General parameter incompatibility reasons */
    CO_SDO_AB_DEVICE_INCOMPAT       = 0x06040047UL, /**< 0x06040047, General internal incompatibility in device */
    CO_SDO_AB_HW                    = 0x06060000UL, /**< 0x06060000, Access failed due to hardware error */
    CO_SDO_AB_TYPE_MISMATCH         = 0x06070010UL, /**< 0x06070010, Data type does not match, length of service parameter does not match */
    CO_SDO_AB_DATA_LONG             = 0x06070012UL, /**< 0x06070012, Data type does not match, length of service parameter too high */
    CO_SDO_AB_DATA_SHORT            = 0x06070013UL, /**< 0x06070013, Data type does not match, length of service parameter too short */
    CO_SDO_AB_SUB_UNKNOWN           = 0x06090011UL, /**< 0x06090011, Sub index does not exist */
    CO_SDO_AB_INVALID_VALUE         = 0x06090030UL, /**< 0x06090030, Invalid value for parameter (download only). */
    CO_SDO_AB_VALUE_HIGH            = 0x06090031UL, /**< 0x06090031, Value range of parameter written too high */
    CO_SDO_AB_VALUE_LOW             = 0x06090032UL, /**< 0x06090032, Value range of parameter written too low */
    CO_SDO_AB_MAX_LESS_MIN          = 0x06090036UL, /**< 0x06090036, Maximum value is less than minimum value. */
    CO_SDO_AB_NO_RESOURCE           = 0x060A0023UL, /**< 0x060A0023, Resource not available: SDO connection */
    CO_SDO_AB_GENERAL               = 0x08000000UL, /**< 0x08000000, General error */
    CO_SDO_AB_DATA_TRANSF           = 0x08000020UL, /**< 0x08000020, Data cannot be transferred or stored to application */
    CO_SDO_AB_DATA_LOC_CTRL         = 0x08000021UL, /**< 0x08000021, Data cannot be transferred or stored to application because of local control */
    CO_SDO_AB_DATA_DEV_STATE        = 0x08000022UL, /**< 0x08000022, Data cannot be transferred or stored to application because of present device state */
    CO_SDO_AB_DATA_OD               = 0x08000023UL, /**< 0x08000023, Object dictionary not present or dynamic generation fails */
    CO_SDO_AB_NO_DATA               = 0x08000024UL  /**< 0x08000024, No data available */
}CO_SDO_abortCode_t;

struct CO_SDO_t {
	CAN_Msg* rx_msg;
        CAN_Msg tx_msg;
	uint16_t server_client_id;
	uint16_t client_server_id;


	CO_SDO_state_t state;
        /** Offset in buffer of next data segment being read/written */
        uint16_t            buffer_offset;
        /** Sequence number of OD entry as returned from CO_OD_find() */
        uint16_t data_len;
        uint16_t            entryNo;
        uint16_t            index;
        uint8_t             sub;
        uint16_t object_data_len;
        uint8_t* object_data_buffer;
        uint8_t             nodeId;
        /** Current internal state of the SDO server state machine #CO_SDO_state_t */
        uint8_t             sequence;
        /** Timeout timer for SDO communication */
        uint32_t        timeout_tmr;
        uint32_t        timeout_time;

        /** Number of segments per block with 1 <= blksize <= 127 */
        uint8_t             blksize;
        /** True, if CRC calculation by block transfer is enabled */
        bool              crcEnabled;
        /** Calculated CRC code */
        uint16_t            crc;
        /** Length of data in the last segment in block upload */
        uint8_t             lastLen;
        /** Indication end of block transfer */
        bool              endOfTransfer;
};


void sdo_init(CO_SDO* p_sdo,const uint16_t client_to_server_id,
		const uint16_t server_to_client_id);

int32_t sdo_process(CO_SDO* p_sdo,CO_OD* p_od,const uint32_t time_diff);
void sdo_abort(CO_SDO* p_sdo,const uint32_t code);

static inline bool sdo_is_timeout(CO_SDO* p_sdo){
        return (p_sdo->timeout_tmr > p_sdo->timeout_time);
}

static inline void sdo_set_msg(CO_SDO* p_sdo,CAN_Msg* p_msg){
	p_sdo->rx_msg=p_msg;
}


#endif
