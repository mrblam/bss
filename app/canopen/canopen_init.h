/*
 * canopen_init.h
 *
 *  Created on: Oct 21, 2020
 *      Author: quangnd
 */

#ifndef APP_CANOPEN_CANOPEN_INIT_H_
#define APP_CANOPEN_CANOPEN_INIT_H_

#include "can_hal.h"
#define BP_NUM                                                  3
#define BP_SN_SIZE                                      32

typedef struct CO_SDO_SERVER_t CO_SDO_SERVER;

typedef enum SDO_STATE_t{
        SDO_ST_IDLE=        0,
        SDO_ST_SENT
}SDO_STATE;
struct CO_SDO_SERVER_t{
        uint32_t timeout;
        SDO_STATE state;
        uint16_t receive_index;
};

typedef enum BP_CONNECT_STATE_t{

        BP_CON_ST_DISCONNECTED=0,
        BP_CON_ST_AUTHORIZING,
        BP_CON_ST_AUTHORIZED
}BP_CONNECT_STATE;

typedef struct BP_PDO_Data_t BP_PDO_Data;
struct BP_PDO_Data_t{
        uint8_t node_id;
        uint8_t auth_state;
        char serial_number[BP_SN_SIZE];
        uint32_t vol;
        int32_t cur;
        uint8_t cell_vol[16];
        int8_t temp[8];
        uint8_t soc;
        uint8_t soh;
        uint8_t state;
        uint8_t status;
        BP_CONNECT_STATE con_state;
        uint32_t inactive_time;
        char* (*serializer)(const BP_PDO_Data* const p_bp,char* buff);
        CO_SDO_SERVER sdo_server;
};

typedef struct VEH_PDO_Data_t{
        char serial_number[8];
        uint32_t speed;
        uint32_t lat;
        uint32_t lon;
}VEH_PDO_Data;


static inline SDO_STATE sdo_server_get_state(const CO_SDO_SERVER* const p_svr){
        return p_svr->state;
}

static inline void sdo_server_set_state(CO_SDO_SERVER* p_svr,const SDO_STATE state){
        p_svr->state=state;
}

extern BP_PDO_Data bp_data[BP_NUM];
extern VEH_PDO_Data veh_data;
static inline BP_CONNECT_STATE bp_get_con_state(const BP_PDO_Data* const p_bp){
        return p_bp->con_state;
}

static inline void bp_set_con_state(BP_PDO_Data* p_bp,const BP_CONNECT_STATE state){
        p_bp->con_state=state;
}
void canopen_service_init(void);
void bp_sdo_server_check_timeout(BP_PDO_Data* p_bp,const uint32_t timestamp);
void bp_send_sync_request(BP_PDO_Data *p_bp, const uint32_t timestamp) ;

#endif /* APP_CANOPEN_CANOPEN_INIT_H_ */
