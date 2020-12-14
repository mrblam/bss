/*
 * CO_PDO.h
 *
 *  Created on: Jun 25, 2019
 *      Author: quangnd
 */

#ifndef CO_PDO_H_
#define CO_PDO_H_
#include "stdint.h"

#define MAX_MAPPED_OBJECTS		0x08
#include "../../../libs/selex-libc/canopen_clib/CAN_Msg.h"
#include "../../../libs/selex-libc/canopen_clib/CO_Object.h"

typedef struct CO_PDO_t CO_PDO;
typedef void (*PDO_Handle)(CO_PDO* p_pdo);
typedef struct PDO_Map_Object_t PDO_Map_Object;

struct PDO_Map_Object_t{
        
        uint16_t index;
        uint8_t sub;
        void* p_data;
        uint8_t len;
};

struct CO_PDO_t{
        CAN_Msg* msg;
	uint16_t cob_id;
        uint8_t mapped_object_no;
        PDO_Map_Object mapped_objects[MAX_MAPPED_OBJECTS];
        PDO_Handle handle;
};

void pdo_init(CO_PDO* p_pdo,const uint16_t cob_id);
void pdo_map_object(CO_PDO* p_pdo,const CO_Object* p_obj,const uint8_t sub,const uint8_t len);

static inline void pdo_set_handle(CO_PDO* p_pdo,PDO_Handle handle){
	p_pdo->handle=handle;
}

static inline void pdo_set_msg(CO_PDO* p_pdo,CAN_Msg* p_msg){
        p_pdo->msg=p_msg;
}

static inline CAN_Msg* pdo_get_msg(CO_PDO* p_pdo){
        return p_pdo->msg;
}

static inline void pdo_handle(CO_PDO* p_pdo){
	if(p_pdo->msg->is_new==false) return;
        p_pdo->handle(p_pdo);
}

#endif /* CO_PDO_H_ */

