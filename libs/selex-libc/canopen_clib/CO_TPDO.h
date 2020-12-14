#ifndef CO_TPDO_H_
#define CO_TPDO_H_
#include "../../../libs/selex-libc/canopen_clib/CO_PDO.h"

typedef struct CO_TPDO_t CO_TPDO;

struct CO_TPDO_t{
        CO_PDO base;
};

void tpdo_init(CO_TPDO* p_tpdo,const uint16_t cob_id);

#endif
