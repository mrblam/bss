/*
 * mqtt_init.c
 *
 *  Created on: Oct 6, 2020
 *      Author: quangnd
 */
#include "mqtt_init.h"
#include "sim800a.h"
#include "MQTTClient.h"
#include "network.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#if 0
/* linode server */
const char *iot_mqtt_server_addr = "139.162.3.35";
#endif
/* office server */
const char *iot_mqtt_server_addr = "117.4.243.74";
const uint32_t iot_mqtt_server_port = 1883;
const char *iot_mqtt_server_username = "selex";
const char *iot_mqtt_server_password = "selex";
const char *cbox_client_id = "S2-CBox";
const char *data_topic = "dt/battery/info";

MQTTClient cbox_mqtt_client;
Sim800a cbox_sim;
static Network network;
//static unsigned char sendbuf[1024], readbuf[128];

static int32_t mqtt_net_read_impl(Network *p_net, char *buff, int32_t len,
		int32_t timeout);
static int32_t mqtt_net_write_impl(Network*, const char*, int32_t, int32_t);
static void mqtt_net_init_impl(Network*);
static int32_t mqtt_net_connnect_impl(Network*, const char*, const uint32_t);
static void mqtt_net_disconnect(Network*);

#if 0
void messageArrived(MessageData* data){
}
#endif
static void sim800a_on_receive(const char*,const uint16_t len);

void mqtt_service_init(void) {

	network.init = mqtt_net_init_impl;
	network.connnect = mqtt_net_connnect_impl;
	network.disconnect = mqtt_net_disconnect;
	network.mqttread = mqtt_net_read_impl;
	network.mqttwrite = mqtt_net_write_impl;
	cbox_sim.base.send = sim800a_sends;
	cbox_sim.base.send_len = sim800a_send_len;
	cbox_sim.base.read_all=sim800a_read_all;
	cbox_sim.base.clear_uart_rx_buffer=sim800a_clear_rx_buffer;

	NetworkInit(&network);
	MQTTClientInit(&cbox_mqtt_client, &network, 30000,
	                cbox_sim.base.tx_data_buff,
	                AT_DATA_TX_BUFFER_SIZE,
			cbox_sim.base.rx_buffer,AT_CMD_RESPONSE_BUFFER_SIZE);
}

int32_t mqtt_service_start(void) {

	int32_t rc = 0;
	if ((rc = NetworkConnect(&network, iot_mqtt_server_addr,
			iot_mqtt_server_port)) <= 0) {
		return rc;
	}

	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = (char*) cbox_client_id;
	data.username.cstring = (char*) iot_mqtt_server_username;
	data.password.cstring = (char*) iot_mqtt_server_password;
	data.keepAliveInterval = 60;
	data.cleansession = 1;

	at_modem_flush_rx_buffer((AT_Modem*)&cbox_sim);
	rc = MQTTConnect(&cbox_mqtt_client, &data);
	return rc;
}

int32_t mqtt_sync_data(const char *data, const int32_t len) {

	int32_t rc = 0;
	MQTTMessage message;
	message.qos = 1;
	//message.qos = 0;
	message.retained = 0;
	message.payload =(char*)data;
	message.payloadlen = len;

	if ((rc = MQTTPublish(&cbox_mqtt_client, data_topic, &message)) != 0) {
		return rc;
	}
	return 0;
}

static void mqtt_net_init_impl(Network *p_net) {

	p_net->my_socket = 0;
	sim800a_init(&cbox_sim);
	sim800a_set_receive_handle(sim800a_on_receive);
	cbox_sim.base.send = sim800a_sends;
	cbox_sim.base.send_len = sim800a_send_len;
}

static int32_t mqtt_net_read_impl(Network *p_net, char *buff, int32_t len,
		int32_t timeout) {

	if (p_net->my_socket < 0)
		return -1;
	if (timeout <= 0)
		return -1;
	Timer timer;
	TimerInit(&timer);
	TimerCountdownMS(&timer, timeout);
	int32_t read=0;

	while ((read<len) && !TimerIsExpired(&timer)) {
	      at_modem_read_all((AT_Modem*)&cbox_sim);
	      read=cbox_sim.base.rx_index-cbox_sim.base.rx_read_index;
	}

	if (TimerIsExpired(&timer)){
	        timer_free(&timer);
		return -1;
	}
	char *rev_buff = (char*) cbox_sim.base.rx_buffer+cbox_sim.base.rx_read_index;
	for (int i = 0; i < len; i++) {
		*buff++ = *rev_buff++;
	}
	cbox_sim.base.rx_read_index+=len;
	timer_free(&timer);
	return len;
}

static int32_t mqtt_net_write_impl(Network *p_net, const char *buff, int32_t len,
		int32_t timeout) {

        AT_Modem* p_mdm=(AT_Modem*)&cbox_sim;
	if (p_net->my_socket < 0)
		return -1;

	if(sim800a_start_data_sending(&cbox_sim) <0){
	        return -2;
	}

	at_modem_flush_rx_buffer(p_mdm);
	Timer timer;
	TimerInit(&timer);
	TimerCountdownMS(&timer, timeout);
	/* start send packgage */
#if 0
	while ((sim800a_send_pack_len(&cbox_sim, buff, len) < 0)
			&& !TimerIsExpired(&timer)) {
	}
#endif
	cbox_sim.base.send_len(buff,len);
	if (TimerIsExpired(&timer)){
	        timer_free(&timer);
		return -3;
	}
	if(sim800a_terminate_data_sending(&cbox_sim) < 0){
	        timer_free(&timer);
	        return -4;
	}
	/* clean buffer for response from server */
	at_modem_flush_rx_buffer((AT_Modem*)&cbox_sim);
	timer_free(&timer);
	return len;
}

static int32_t mqtt_net_connnect_impl(Network *p_net, const char *addr,
		const uint32_t port) {

        char ip[32];
        int32_t rc = -1;
        if (!sim800a_is_active(&cbox_sim))
        {
                //sim800a_tcp_shutdown(&cbox_sim);
                sim800a_terminate_data_sending(&cbox_sim);
                return -1;
        }
        rc = sim800a_turnoff_echo(&cbox_sim);
        if (rc < 0)
                return -2;

        cbox_sim.client_state = SIM800_CLIENT_ST_PDP_SETUP;

        rc = sim800a_update_client_state(&cbox_sim);
        if (rc < 0)
                return -4;

        if(cbox_sim.client_state==SIM800_CLIENT_ST_CONNECT_OK){
                p_net->my_socket=1;
                return 1;
        }

        switch (cbox_sim.client_state) {
        case SIM800_CLIENT_ST_IP_INITIAL:
                rc = sim800a_set_apn(&cbox_sim, "CMNET", "", "");
                if (rc < 0)
                        return -5;
                break;
        case SIM800_CLIENT_ST_IP_START:
                rc = sim800a_active_gprs(&cbox_sim);
                if (rc < 0)
                        return -6;
                break;
        case SIM800_CLIENT_ST_PDP_DEACT:
                rc=sim800a_tcp_shutdown(&cbox_sim);
                if(rc<0)
                        return -10;
                rc = sim800a_connect_gprs(&cbox_sim);
                if (rc < 0)
                        return -10;
                break;
        case SIM800_CLIENT_ST_IP_GPRSACT:
                rc = sim800a_get_local_ip(&cbox_sim, ip);
                if (rc < 0)
                        return -7;
                break;
        case SIM800_CLIENT_ST_IP_STATUS:
                rc = sim800a_connect_tcp_server(&cbox_sim, addr, port);
                if (rc < 0)
                        return -8;
                break;
        case SIM800_CLIENT_ST_CLOSED:
                rc = sim800a_connect_tcp_server(&cbox_sim, addr, port);
                if (rc < 0)
                        return -9;
                break;
        case SIM800_CLIENT_ST_TCP_CONNECTING:
                return -9;
        default:
                return -5;
        }
        return -10;
}

static void mqtt_net_disconnect(Network *p_net) {

	p_net->my_socket = -1;
	sim800a_close_tcp_connect(&cbox_sim);
}

static void sim800a_on_receive(const char* ch,const uint16_t len){
        memcpy(cbox_sim.base.rx_buffer+cbox_sim.base.rx_index,ch,len);
        cbox_sim.base.rx_index+=len;
}
