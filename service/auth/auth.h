/*
 * auth.h
 *
 *  Created on: Oct 20, 2020
 *      Author: nguyenquang
 */

#ifndef SERVICE_AUTH_AUTH_H_
#define SERVICE_AUTH_AUTH_H_

#include "stdint.h"

typedef struct Auth_t Auth;

struct Auth_t{

        uint8_t (*check)(Auth* p_auth,const char* id,const char* secret);
};

static inline uint8_t auth_check(Auth* p_auth,const char* id,const char* secret){
        return p_auth->check(p_auth,id,secret);
}


#endif /* SERVICE_AUTH_AUTH_H_ */
