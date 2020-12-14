/*
 * CO_Object.h
 *
 *  Created on: Jun 25, 2019
 *      Author: quangnd
 */

#ifndef CO_OBJECT_H_
#define CO_OBJECT_H_

#include "stdint.h"

#include "../../../libs/selex-libc/canopen_clib/CO_utils.h"

typedef struct CO_Object_t CO_Object;
typedef struct CO_Object_Sub_t CO_Object_Sub;

struct CO_Object_t{
	uint16_t index;
        uint8_t sub_no;
        CO_Object_Sub* subs;
};

struct CO_Object_Sub_t{
        void* p_data;
        uint8_t attr;
        uint8_t len;
};

static inline void co_obj_write(CO_Object* p_obj,const uint8_t sub,const void* data,const uint8_t len){

        uint8_t* src= (uint8_t*) p_obj->subs[sub].p_data;
        uint8_t* dst= (uint8_t*) data;
        CO_memcpy(dst,src,len);
}

static inline void co_obj_read(CO_Object* p_obj,const uint8_t sub,void* data,const uint8_t len){

        uint8_t* src= (uint8_t*) p_obj->subs[sub].p_data;
        uint8_t* dst= (uint8_t*) data;
        CO_memcpy(dst,src,len);
}

static inline void* co_obj_get_data_pointer(CO_Object* p_obj,const uint8_t sub){

	return p_obj->subs[sub].p_data;
}

static inline uint8_t co_obj_get_data_len(CO_Object* p_obj,const uint8_t sub){

	return p_obj->subs[sub].len;
}

static inline uint8_t co_obj_get_sub_attr(CO_Object* p_obj,const uint8_t sub){

	return p_obj->subs[sub].attr;
}


#endif /* CO_OBJECT_H_ */
