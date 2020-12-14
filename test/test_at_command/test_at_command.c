/*
 * test_at_command.c
 *
 *  Created on: Oct 10, 2020
 *      Author: quangnd
 */

#include "stdint.h"
#include "at_modem.h"
#include "stdio.h"
#include "stdlib.h"
#include "sim800a.h"
#include "mqtt_init.h"
#include "string_util.h"

static void test_build_connect_package(void);
static void test_publish_topic(void);
static AT_Modem modem;
const char* test_str="s2/bat/data";

static inline void fill_test_data(void){

	while(*test_str){
		at_modem_receive_handle(&modem,*test_str);
		test_str++;
	}
}
static void test_init(void){

	at_modem_init(&modem);

	fill_test_data();
	at_modem_flush_rx_buffer(&modem);
	fill_test_data();
}

static void test_send_pack(void){

        Sim800a sim;
	sim800a_init(&sim);
	sim.base.send=sim800a_sends;
	sim.base.send_len=sim800a_send_len;

	sim800a_send_pack(&sim, "Nguyen Dinh Quang");
	sim800a_send_pack_len(&sim, "Nguyen Dinh Quang", 10);
	sim800a_connect_tcp_server(&sim, "192.132.223.21", 1884);
	sim800a_close_tcp_connect(&sim);

}

static void test_mqtt_service(void){

	mqtt_service_init();
	mqtt_service_start();

}

static int32_t sim800a_readall_mock(char* buff);
static void test_at_command_parse(void){
        Sim800a sim;
	sim800a_init(&sim);
	at_modem_flush_rx_buffer(&sim);
	sim.base.read_all=sim800a_readall_mock;
	sim.base.rx_index+=sim.base.read_all(sim.base.rx_buffer+sim.base.rx_index);

        at_modem_search_response_string(&sim,"OK\r\n");
}

static int32_t sim800a_readall_mock(char* buff){
        const char* test="\r\nOK\r\n";
        for(int i=0;i<6;i++){
                *buff++=test[i];
        }
        return 6;
}

int32_t main(void){

	//test_init();
	//test_at_command_parse();
	//test_mqtt_service();
//	test_send_pack();
	//test_build_connect_package();
        test_publish_topic();
}

Timer_Hw* timer_hw_create(void){
	return (Timer_Hw*)malloc(sizeof(Timer_Hw));
}

void sim800a_send(const char ch){

	printf("%s\n",&ch);
}

void sim800a_sends(const char* s){
	printf(s);
}

void sim800a_send_len(const char* s,const int32_t len){

	for(int i=0;i<len;i++){
		printf("%s",s+i);
	}
}

static char pack_buffer[256]={0};
static uint16_t buff_index=0;
static void send_string_mock(const char* s);
static void send_char_mock(const char c);

static void test_build_connect_package(void){

        buff_index=0;
        memset(pack_buffer,0,256);
        const char *cbox_client_id = "S2-CBox";
        const char *data_topic = "bat/serial/data";
        unsigned char MQTTProtocolNameLength;
        unsigned char MQTTClientIDLength;
        unsigned char MQTTUsernameLength;
        unsigned char MQTTPasswordLength;
        unsigned char Remain_Length,Encoded_Byte;
        char MQTTHost[15] = "139.162.3.35";
        char MQTTPort[5] = "1883";
        char MQTTClientID[10] = "S2-CBox";
        char MQTTTopic[60] = "bat/serial/data";
        char MQTTProtocolName[8] = "MQIsdp";
        char MQTTLVL = 0x03;
        unsigned char MQTTFlags = 0xC2;
        char MQTTKeepAlive = 60;
        char MQTTUsername[6] = "selex";
        char MQTTPassword[6] = "selex";
        unsigned short topiclength,messagelength;
        char topic[60],message[100];
        char str[200];
        MQTTProtocolNameLength = strlen(MQTTProtocolName);
                MQTTClientIDLength = strlen(MQTTClientID);
                MQTTUsernameLength = strlen(MQTTUsername);
                MQTTPasswordLength = strlen(MQTTPassword);
                Remain_Length = MQTTProtocolNameLength+2+4+MQTTClientIDLength+2+MQTTUsernameLength+2+MQTTPasswordLength+2;

                send_char_mock(0x10);
                do
                {
                        Encoded_Byte = Remain_Length % 128;
                        Remain_Length = Remain_Length /128;
                        if (Remain_Length>0)
                        {
                                Encoded_Byte|=128;
                        }
                        send_char_mock(Encoded_Byte);
                }while (Remain_Length>0);
                send_char_mock(MQTTProtocolNameLength>>8);
                send_char_mock(MQTTProtocolNameLength&0xFF);
                send_string_mock(MQTTProtocolName);
                send_char_mock(MQTTLVL);
                send_char_mock(MQTTFlags);
                send_char_mock(MQTTKeepAlive>>8);
                send_char_mock(MQTTKeepAlive&0xFF);
                send_char_mock(MQTTClientIDLength>>8);
                send_char_mock(MQTTClientIDLength&0xFF);
                send_string_mock(MQTTClientID);
                send_char_mock(MQTTUsernameLength>>8);
                send_char_mock(MQTTUsernameLength&0xFF);
                send_string_mock(MQTTUsername);
                send_char_mock(MQTTPasswordLength>>8);
                send_char_mock(MQTTPasswordLength&0xFF);
                send_string_mock(MQTTPassword);
                send_char_mock(0x1A);

}

static void test_publish_topic(void) {

        buff_index=0;
        memset(pack_buffer,0,256);

        const char* s="speed:1000";
        const char *cbox_client_id = "S2-CBox";
        const char *data_topic = "bat/serial/data";
        unsigned char MQTTProtocolNameLength;
        unsigned char MQTTClientIDLength;
        unsigned char MQTTUsernameLength;
        unsigned char MQTTPasswordLength;
        unsigned char Remain_Length,Encoded_Byte;
        char MQTTHost[15] = "139.162.3.35";
        char MQTTPort[5] = "1883";
        char MQTTClientID[10] = "S2-CBox";
        char MQTTTopic[60] = "bat/serial/data";
        char MQTTProtocolName[8] = "MQIsdp";
        char MQTTLVL = 0x03;
        unsigned char MQTTFlags = 0xC2;
        char MQTTKeepAlive = 60;
        char MQTTUsername[6] = "selex";
        char MQTTPassword[6] = "selex";
        unsigned short topiclength,messagelength;
        char topic[60],message[100];
        char str[200];

        send_char_mock(0x30);
        topiclength = sprintf((char*) topic, MQTTTopic);
        messagelength = sprintf((char*) message, s);
        Remain_Length = sprintf((char*) str, "%s%s", topic, message);
        Remain_Length += 2;
        do {
                Encoded_Byte = Remain_Length % 128;
                Remain_Length = Remain_Length / 128;
                if (Remain_Length > 0) {
                        Encoded_Byte |= 128;
                }
                send_char_mock(Encoded_Byte);
        } while (Remain_Length > 0);
        send_char_mock(topiclength >> 8);
        send_char_mock(topiclength & 0xFF);
        send_string_mock((uint8_t*) str);
}

static void send_char_mock(const char c){
        pack_buffer[buff_index++]=c;
}

static void send_string_mock(const char* s){
        while(*s){
               pack_buffer[buff_index++]=*s;
               s++;
        }
}

