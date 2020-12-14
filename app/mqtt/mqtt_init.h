/*
 * mqtt_init.h
 *
 *  Created on: Oct 6, 2020
 *      Author: quangnd
 */

#ifndef APP_MQTT_MQTT_INIT_H_
#define APP_MQTT_MQTT_INIT_H_
#include "MQTTClient.h"
#include "sim800a.h"

extern MQTTClient cbox_mqtt_client;
extern Sim800a cbox_sim;
extern const char* iot_mqtt_server_addr;
extern const uint32_t iot_mqtt_server_port;
extern const char* iot_mqtt_server_username;
extern const char*iot_mqtt_server_password;
extern const char* cbox_client_id;
extern const char* data_topic;

void mqtt_service_init(void);
int32_t mqtt_service_start(void);
int32_t mqtt_sync_data(const char *data, const int32_t len) ;

#endif /* APP_MQTT_MQTT_INIT_H_ */
