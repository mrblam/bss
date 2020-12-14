#include "../../../libs/selex-libc/canopen_clib/CO_TPDO.h"

static void tpdo_process(CO_PDO* p_tpdo);

void tpdo_init(CO_TPDO* p_tpdo,const uint16_t cob_id){
        pdo_init((CO_PDO*)p_tpdo,cob_id);
        pdo_set_handle((CO_PDO*)p_tpdo,tpdo_process);
}

static void tpdo_process(CO_PDO* p_tpdo){

        CAN_Msg* p_msg= pdo_get_msg((CO_PDO*)p_tpdo);
        uint8_t* msg_data= (uint8_t*) p_msg->data;

        uint8_t* obj_data;
        uint8_t mapped_obj_id=0;
        uint8_t byte_index=0;
        for(mapped_obj_id=0;mapped_obj_id<p_tpdo->mapped_object_no; mapped_obj_id++){
                obj_data= (uint8_t*)p_tpdo->mapped_objects[mapped_obj_id].p_data;
                for(byte_index=0;byte_index<p_tpdo->mapped_objects[mapped_obj_id].len;byte_index++){
                        *msg_data++ = *obj_data++;
                }
        }
}
