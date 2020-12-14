/*
 * json.h
 *
 *  Created on: Oct 7, 2020
 *      Author: quangnd
 */

#ifndef UTIL_JSON_JSON_H_
#define UTIL_JSON_JSON_H_
#include "stdint.h"

char* json_from_string(char* buff,const char* name,const char* str);
char* json_from_long(char* buff,const char* name,const uint32_t val);
char* json_from_slong(char* buff,const char* name, const int32_t val);
char* json_open(char* buff);
char* json_close(char* buff);
char* json_from_long_array(char* buff,const char* name,const uint32_t* val,const uint16_t size);
char* json_from_byte_array(char* buff,const char* name,const uint8_t* val,const uint16_t size);

#endif /* UTIL_JSON_JSON_H_ */
