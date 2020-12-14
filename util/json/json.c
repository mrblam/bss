/*
 * json.c
 *
 *  Created on: Oct 7, 2020
 *      Author: quangnd
 */
#include "json.h"
#include "string_util.h"
#include "string.h"

char* json_open(char* buff){
        *buff++='{';
        return buff;
}

char* json_close(char* buff){
        *buff++='}';
        *buff='\0';
        return buff;
}

char* json_from_string(char* buff,const char* name,const char* str){

        *buff++='"';
        while(*name){
                *buff++=*name++;
        }
        *buff++='"';
        *buff++=':';
        *buff++='"';
        while(*str){
                *buff++=*str++;
        }
        *buff++='"';
        return buff;
}

char* json_from_long(char* buff,const char* name,const uint32_t val){

	char val_buff[32]={0};
	long_to_string(val,(uint8_t*) val_buff);
	*buff++='"';
	while(*name){
	        *buff++=*name++;
	}
	*buff++='"';
	*buff++=':';
	char* val_str=val_buff;
	while(*val_str){
	        *buff++=*val_str++;
	}
	return buff;
}

char* json_from_slong(char* buff,const char* name,const int32_t val){

	if(val<0) {
		return json_from_long(buff,name,-val);
	}
	return json_from_long(buff,name,val);
}

char* json_from_long_array(char* buff,const char* name,const uint32_t* val,const uint16_t size){

        char val_buff[16]={0};
	char* val_str;
	*buff++='"';
	while(*name){
	        *buff++=*name++;
	}
	*buff++='"';
	*buff++=':';
	*buff++='[';
	for(int i=0;i<size;i++){
	        long_to_string(val[i],(uint8_t*) val_buff);
	        val_str=val_buff;
	        while(*val_str){
	                *buff++=*val_str++;
	        }
	        *buff++=',';
	}
	buff--;
	*buff++=']';
	return buff;
}

char* json_from_byte_array(char* buff,const char* name,const uint8_t* val,const uint16_t size){

        char val_buff[16]={0};
	char* val_str;
	*buff++='"';
	while(*name){
	        *buff++=*name++;
	}
	*buff++='"';
	*buff++=':';
	*buff++='[';
	for(int i=0;i<size;i++){
	        long_to_string(val[i],(uint8_t*) val_buff);
	        val_str=val_buff;
	        while(*val_str){
	                *buff++=*val_str++;
	        }
	        *buff++=',';
	}
	buff--;
	*buff++=']';
	return buff;
}




