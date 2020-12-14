#ifndef CO_OD_H_
#define CO_OD_H_
#include "stdint.h"
#include "stdlib.h"
#include "../../../libs/selex-libc/canopen_clib/CO_Object.h"

#define OD_UNSUPPORT_ENTRY	0xFFFFU

typedef struct CO_OD_t CO_OD;

struct CO_OD_t{
        CO_Object* entries;
        uint16_t size;
};

void od_write(CO_OD* p_od,const uint16_t entry,const uint8_t sub,const void* data,const uint8_t len);

static inline uint16_t od_get_entry(const CO_OD* p_od,const uint16_t index){

    /* Fast search in ordered Object Dictionary. If indexes are mixed, this won't work. */
    /* If Object Dictionary has up to 2^N entries, then N is max number of loop passes. */
    uint16_t cur, min, max;
    uint16_t od_size=p_od->size;
    const CO_Object* p_obj;

    min = 0U;
    max = od_size-1U;
    while(min < max){
        cur = (min + max) >>1;
        p_obj=&p_od->entries[cur];
        /* Is object matched */
        if(index == p_obj->index){
            return cur;
        }
        if(index < p_obj->index){
            max = cur;
            if(max) max--;
        }
        else
            min = cur + 1U;
    }

    if(min == max){
        p_obj = &p_od->entries[min];
        /* Is object matched */
        if(index == p_obj->index){
            return min;
        }
    }

    return OD_UNSUPPORT_ENTRY;  /* object does not exist in OD */
}

static inline void* od_get_data_pointer(const CO_OD* p_od,
                                const uint16_t entry_no,const uint8_t sub){
	if(entry_no > p_od->size) return NULL;

	return co_obj_get_data_pointer(&p_od->entries[entry_no],sub);
}

static inline uint8_t od_get_data_len(const CO_OD* p_od,
                                const uint16_t entry_no,const uint8_t sub){

	if(entry_no > p_od->size) return 0;
	return co_obj_get_data_len(&p_od->entries[entry_no],sub);
}

#endif


