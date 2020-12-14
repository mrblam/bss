#ifndef CO_RPDO_H_
#define CO_RPDO_H_
#include "../../../libs/selex-libc/canopen_clib/CO_PDO.h"

typedef struct CO_RPDO_t CO_RPDO;

struct CO_RPDO_t{
        CO_PDO base;
};

void rpdo_init(CO_RPDO* p_rpdo,const uint16_t cob_id);

#endif
