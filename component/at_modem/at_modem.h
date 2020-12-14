/*
 * at_modem.h
 *
 *  Created on: Oct 7, 2020
 *      Author: quangnd
 */

#ifndef COMPONENT_AT_MODEM_AT_MODEM_H_
#define COMPONENT_AT_MODEM_AT_MODEM_H_
#include "stdint.h"
#include "string.h"

#define AT_CMD_RESPONSE_BUFFER_SIZE                     128
#define AT_CMD_TX_BUFFER_SIZE                           32
#define AT_DATA_TX_BUFFER_SIZE                          1024

typedef struct AT_Modem_t AT_Modem;

typedef enum AT_MODEM_CMD_STATE_t{
	MODEM_ST_IDLE=1,
	MODEM_ST_WAIT_FOR_RESPONSE,
	MODEM_ST_RESPONSE_FINISH,
	MODEM_ST_TIMEOUT
}AT_MODEM_CMD_STATE;

typedef enum AT_MODEM_STATE_t{

        AT_MODEM_ST_INACTIVE=0,
        AT_MODEM_ST_ACTIVE
}AT_MODEM_STATE;

struct AT_Modem_t{
	AT_MODEM_CMD_STATE cmd_state;
	AT_MODEM_STATE state;
	uint8_t rx_buffer[AT_CMD_RESPONSE_BUFFER_SIZE];
	int16_t rx_index;
	int16_t rx_read_index;
	uint8_t tx_cmd[AT_CMD_TX_BUFFER_SIZE];
	uint8_t tx_data_buff[AT_DATA_TX_BUFFER_SIZE];
	void (*send)(const char* cmd);
	void (*send_len)(const char* cmd,const int32_t len);
	int32_t (*read_all)(char* buff);
	void (*clear_uart_rx_buffer)(void);
};


void at_modem_init(AT_Modem* p_modem);
int32_t at_modem_send_cmd(AT_Modem* p_modem,const char* res,const uint32_t timeout);
int32_t at_modem_search_response_string(AT_Modem* p_modem,const char* str);

static inline void at_modem_set_cmd(AT_Modem* p_modem,const char* cmd){

	char* cmd_buff=(char*)p_modem->tx_cmd+3;
	while(*cmd){
		*cmd_buff=*cmd;
		cmd++;
		cmd_buff++;
	}
	*cmd_buff++='\r';
	*cmd_buff++='\n';
	*cmd_buff++='\0';
}

static inline void at_modem_flush_rx_buffer(AT_Modem* p_modem){

        p_modem->clear_uart_rx_buffer();
        memset(p_modem->rx_buffer,0,AT_CMD_RESPONSE_BUFFER_SIZE);
	p_modem->rx_index=0;
	p_modem->rx_read_index=0;
	//p_modem->rx_buffer[0]='\0';
}

static inline void at_modem_receive_handle(AT_Modem* p_modem,const char ch){

	p_modem->rx_buffer[p_modem->rx_index++]=ch;
	p_modem->rx_buffer[p_modem->rx_index]='\0';
	if(p_modem->rx_index==AT_CMD_RESPONSE_BUFFER_SIZE){
		p_modem->rx_index=0;
	}
}

static inline void at_modem_read_all(AT_Modem* p_mdm){
        p_mdm->rx_index+= p_mdm->read_all((char*)(p_mdm->rx_buffer+p_mdm->rx_index));
}

#endif /* COMPONENT_AT_MODEM_AT_MODEM_H_ */
