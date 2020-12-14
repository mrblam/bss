#include "../../../libs/selex-libc/canopen_clib/CO_OD.h"

#include "stdlib.h"

#include "../../../libs/selex-libc/canopen_clib/CO_Object.h"

void od_write(CO_OD* p_od,const uint16_t entry,const uint8_t sub,const void* data,const uint8_t len){

	CO_Object* p_obj=&p_od->entries[entry];
        co_obj_write(p_obj,sub,data,len);
}
