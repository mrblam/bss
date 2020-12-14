/*
 * main.c
 *
 *  Created on: Aug 18, 2020
 *      Author: quangnd
 */
#include "board.h"
#include "delay.h"
#include "can_hal.h"
#include "string_util.h"
#include "mqtt_init.h"
#include "canopen_init.h"
#include "json.h"
#include "app_config.h"

static uint32_t sys_timestamp=0;

static char sync_data[512]={0};
static int32_t data_len=0;
static volatile uint32_t sync_counter=0;
static char* json_serialize(const BP_PDO_Data* const p_bp,char* buff);
static char* csv_serialize(const BP_PDO_Data* const p_bp,char* buff);

static int32_t uart_sync_data(const char* buff,const int32_t len);

static int32_t (*cbox_sync_data)(const char* buff,const int32_t len);

static int32_t bp_data_serialize(char* buff);

#define USE_MQTT

static void app_init(void){
	global_interrupt_disable();
	board_init();
	canopen_service_init();
#ifdef USE_MQTT
	mqtt_service_init();
	cbox_sync_data=mqtt_sync_data;
	bp_data[0].serializer=json_serialize;
	bp_data[1].serializer=json_serialize;
	bp_data[2].serializer=json_serialize;
#else
	cbox_sync_data=uart_sync_data;
	bp_data[0].serializer=csv_serialize;
	bp_data[1].serializer=csv_serialize;
	bp_data[2].serializer=csv_serialize;
#endif

	global_interrupt_enable();
}

int main(void){
	app_init();
	while(1){
	}
	return 0;
}

void HAL_STATE_MACHINE_UPDATE_TICK(void) {

        sys_timestamp+=APP_STATE_MACHINE_UPDATE_TICK_mS;

        sync_counter++;
#ifdef USE_MQTT
        if(cbox_mqtt_client.isconnected==0){
                //sync_counter = 0;
                //mqtt_service_start();
                //return;
        }
#endif

        if(sync_counter==9){
                data_len=bp_data_serialize(sync_data);
        }

        if (sync_counter>=10) {
                /* has data to sync to server */
                if(data_len>0){
                        //cbox_sync_data(sync_data,data_len);
                }
                sync_counter=0;
        }

        for(int i=0;i<BP_NUM;i++){
                bp_data[i].inactive_time+=APP_STATE_MACHINE_UPDATE_TICK_mS;
                if(bp_data[i].inactive_time>BP_INACTIVE_TIMEOUT_mS){
                        /* avoid overflow */
                        bp_data[i].inactive_time=BP_INACTIVE_TIMEOUT_mS;
                        bp_data[i].con_state=BP_CON_ST_DISCONNECTED;
                }
        }
}

static char* json_serialize(const BP_PDO_Data* const p_bp,char* buff){

        buff=json_open(buff);
        buff=json_from_string(buff,"serial_number",p_bp->serial_number);
        *buff++=',';
        buff=json_from_long(buff,"vol",p_bp->vol);
        *buff++=',';
        buff=json_from_slong(buff,"cur",p_bp->cur);
        *buff++=',';
        buff=json_from_byte_array(buff,"cell_vol",p_bp->cell_vol,16);
        *buff++=',';
        buff=json_from_byte_array(buff,"temp",p_bp->temp,5);
        buff=json_close(buff);
        return buff;
}
static char* csv_serialize(const BP_PDO_Data *const p_bp, char *s) {
        *s = ':';
        s++;
        s += long_to_string(p_bp->vol, s);
        *s = ',';
        s++;
        s += slong_to_string(p_bp->cur, s);
        *s = ',';
        s++;
        s += slong_to_string(p_bp->status, s);
        *s = ',';
        for (int i = 0; i < 16; i++) {
                s++;
                s += long_to_string(p_bp->cell_vol[i], s);
                *s = ',';
        }
        for (int j = 0; j < 5; j++) {
                s++;
                s += slong_to_string(p_bp->temp[j],s);
                *s = ',';
        }
        *s = '*';
        s++;
        *s = '\n';
        s++;
        *s = '\0';
        return s;
}
static int32_t bp_data_serialize(char* buff){

        char* temp=buff;

        /* mark to clear old data */
        *buff='\0';
        for(int i=0;i<BP_NUM;i++){
                if(bp_data[i].con_state==BP_CON_ST_AUTHORIZED){
                        buff=bp_data[i].serializer(&bp_data[i],buff);
                }
                else{
                        bp_send_sync_request(&bp_data[i],sys_timestamp);
                }
        }
        return strlen(temp);
}

static int32_t uart_sync_data(const char* buff,const int32_t len){
        sim800a_send_len(buff,len);
        return len;
}
