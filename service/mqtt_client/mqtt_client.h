/*
 * mqtt_client.h
 *
 *  Created on: Oct 6, 2020
 *      Author: quangnd
 */

#ifndef SERVICE_MQTT_CLIENT_MQTT_CLIENT_H_
#define SERVICE_MQTT_CLIENT_MQTT_CLIENT_H_
#include "stdint.h"

typedef struct MQTT_Client_t MQTT_Client;
typedef struct MQTT_Client_Interface_t MQTT_Client_Interface;
typedef struct MQTT_Topic_t MQTT_Topic;

struct MQTT_Topic_t{

	const char* id;
	char content[125];
};

struct MQTT_Client_Interface_t{
	int32_t (*connect)(MQTT_Client* p_mqtt,const char* addr,const uint32_t port);
	int32_t (*disconnect)(MQTT_Client* p_mqtt);
	int32_t (*publish)(MQTT_Client* p_mqtt,const char* topic);
	int32_t (*subcribe)(MQTT_Client* p_mqtt,const char* topic);
	int32_t (*unsubcribe)(MQTT_Client* p_mqtt,const char* topic);
	void (*on_message)(MQTT_Client* p_mqtt,const char* topic,const char* msg);
};

struct MQTT_Client_t{

	MQTT_Client_Interface interface;
};

static inline void mqtt_client_set_interface(MQTT_Client* p_client,MQTT_Client_Interface interface){

	p_client->interface=interface;
}



#endif /* SERVICE_MQTT_CLIENT_MQTT_CLIENT_H_ */
