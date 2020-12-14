/*
 * test_json.c
 *
 *  Created on: Oct 16, 2020
 *      Author: nguyenquang
 */

#include "json.h"

int main(void){
        uint32_t vol=65440;
        int32_t cur=-43000;
        const char* sn="SBP_31_0001";
        char content[1024];
        char* buff=content;
        buff=json_open(buff);
        buff=json_from_string(buff, "serial_number", sn);
        *buff++=',';
        buff=json_from_long(buff, "voltage", vol);
        *buff++=',';
        buff=json_from_slong(buff, "current", cur);
        buff=json_close(buff);
        printf(content);
}
