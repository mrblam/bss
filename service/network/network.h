/*
 * network.h
 *
 *  Created on: Oct 8, 2020
 *      Author: quangnd
 */

#ifndef SERVICE_NETWORK_NETWORK_H_
#define SERVICE_NETWORK_NETWORK_H_
#include "stdint.h"

typedef struct Network_t Network;
struct Network_t
{
	int32_t my_socket;
	int32_t (*mqttread) (Network*,char*,int32_t, int32_t);
	int32_t (*mqttwrite) (Network*,const char*, int32_t, int32_t);
	void (*init)(Network*);
	int32_t (*connnect)(Network*,const char*,const uint32_t);
	void (*disconnect)(Network*);
};

static inline void NetworkInit(Network* p_net){
	p_net->init(p_net);
}

static inline int32_t NetworkConnect(Network* p_net, const char* addr,const uint32_t port){

	return p_net->connnect(p_net,addr,port);
}

static inline void NetworkDisconnect(Network* p_net){
	p_net->disconnect(p_net);
}

#endif /* SERVICE_NETWORK_NETWORK_H_ */
