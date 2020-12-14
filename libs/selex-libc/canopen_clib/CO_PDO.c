/*
 * CO_PDO.c
 *
 *  Created on: Jun 25, 2019
 *      Author: quangnd
 */

#include "../../../libs/selex-libc/canopen_clib/CO_PDO.h"

#include "stdlib.h"

#include "../../../libs/selex-libc/canopen_clib/CO_Object.h"

void pdo_init(CO_PDO* p_pdo,const uint16_t cob_id){

        if(p_pdo==NULL) return;
        p_pdo->cob_id=cob_id;
}

void pdo_map_object(CO_PDO* p_pdo,const CO_Object* p_obj,const uint8_t sub,const uint8_t len){

        if(p_pdo==NULL) return;

        if(p_pdo->mapped_object_no >=MAX_MAPPED_OBJECTS) return;

        PDO_Map_Object* map_obj= &p_pdo->mapped_objects[p_pdo->mapped_object_no];
        
        map_obj->index=p_obj->index;
        map_obj->len=len;
        map_obj->sub=sub;
        map_obj->p_data=p_obj->subs[sub].p_data;
        p_pdo->mapped_object_no++;
}
