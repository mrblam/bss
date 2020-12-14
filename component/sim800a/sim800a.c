/*
 * sim800a.c
 *
 *  Created on: Oct 6, 2020
 *      Author: quangnd
 */

#include "sim800a.h"
#include "at_modem.h"
#include "string.h"
#include "string_util.h"
#include "timer.h"

#define CMD_TERMINATE(buff) *(buff)++='\r'; *(buff)++='\n'; *(buff)++='\0';
#define CMD_START(buff) *(buff)++='A';*(buff++)='T';*(buff++)='+';

void sim800a_init(Sim800a *p_sim) {
	at_modem_init((AT_Modem*) p_sim);
}

int32_t sim800a_set_transfer_mode(Sim800a *p_sim, SIM800_TRANSFER_MODE mode) {

        (void)p_sim;
        (void)mode;
	return 0;
}

uint8_t sim800a_is_active(Sim800a* p_sim){

	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	p_mdm->tx_cmd[0]='A';
	p_mdm->tx_cmd[1]='T';
	p_mdm->tx_cmd[2]='\r';
	p_mdm->tx_cmd[3]='\n';
	p_mdm->tx_cmd[4]='\0';

	int32_t rc=at_modem_send_cmd(p_mdm,"OK\r\n",1000);
	if(rc<0)
	        return 0;
	return 1;
}

int32_t sim800a_turnoff_echo(Sim800a* p_sim){
	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	p_mdm->tx_cmd[0]='A';
	p_mdm->tx_cmd[1]='T';
	p_mdm->tx_cmd[2]='E';
	p_mdm->tx_cmd[3]='0';
	p_mdm->tx_cmd[4]='\r';
	p_mdm->tx_cmd[5]='\n';
	p_mdm->tx_cmd[6]='\0';

	return at_modem_send_cmd(p_mdm,"OK\r\n",1000);
}

int32_t sim800a_connect_gprs(Sim800a *p_sim) {

	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char *cmd_buffer=(char*)(p_mdm->tx_cmd);
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	const char* str=AT_CMD_CONNECT_GPRS;
	while(*str){
	        *cmd_buffer++=*str++;
	}
	*cmd_buffer++='=';
	*cmd_buffer++='1';
	CMD_TERMINATE(cmd_buffer);
	return at_modem_send_cmd(p_mdm,"OK\r\n",3000);
}

int32_t sim800a_disconnect_gprs(Sim800a* p_sim){
	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char *cmd_buffer=(char*)(p_mdm->tx_cmd);
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	const char* str=AT_CMD_CONNECT_GPRS;
	while(*str){
	        *cmd_buffer++=*str++;
	}
	*cmd_buffer++='=';
	*cmd_buffer++='0';
	CMD_TERMINATE(cmd_buffer);
	return at_modem_send_cmd(p_mdm,"OK\r\n",3000);
}

int32_t sim800a_set_apn(Sim800a* p_sim,const char* apn,const char* user,const char* pass){

        AT_Modem *p_mdm = (AT_Modem*) p_sim;
        char *cmd_buffer=(char*)(p_mdm->tx_cmd);
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	const char* str=AT_CMD_SET_APN;
	while(*str){
	        *cmd_buffer++=*str++;
	}
#if 0
	*cmd_buffer++='=';
	*cmd_buffer++='"';
	while(*apn){
	        *cmd_buffer++=*apn++;
	}
	*cmd_buffer++='"';
#endif
	CMD_TERMINATE(cmd_buffer);
	return at_modem_send_cmd(p_mdm,"OK\r\n",3000);

}

int32_t sim800a_active_gprs(Sim800a* p_sim){

	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char *cmd_buffer = (char*) p_mdm->tx_cmd;
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	char *str = (char*) AT_CMD_ACTIVE_GPRS;
	while (*str) {
		*cmd_buffer++ = *str++;
	}

	CMD_TERMINATE(cmd_buffer);
	return at_modem_send_cmd(p_mdm,"OK\r\n",3000);
}

int32_t sim800a_get_local_ip(Sim800a *p_sim, char *ip) {

	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	int32_t rc=-1;
	char *cmd_buffer = (char*) p_mdm->tx_cmd;
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	char *str = (char*) AT_CMD_GET_LOCAL_IP;
	while (*str) {
		*cmd_buffer++ = *str++;
	}

	CMD_TERMINATE(cmd_buffer);
	rc=at_modem_send_cmd(p_mdm,".",2000);

	/* detect ip address pattern : xxx.xxx.xxx.xxx
	 * */
	if(rc<0) return rc;

	for (int i = 0; i <= p_mdm->rx_index; i++, ip++) {
		*ip = p_mdm->rx_buffer[i];
	}
	return 0;
}

int32_t sim800a_connect_tcp_server(Sim800a *p_sim, const char *server,
		const uint32_t port) {
	/*  AT+CIPSTART=”TCP”,”m10.cloudmqtt.com”,”17434″ */
	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char port_buff[8];
	char *cmd_buffer=(char*)(p_mdm->tx_cmd);
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';

	long_to_string(port, (uint8_t*) port_buff);
	strcpy(cmd_buffer, AT_CMD_TCP_CONNECT);
	strcat(cmd_buffer, "=\"TCP\",\"");
	strcat(cmd_buffer, server);
	strcat(cmd_buffer, "\",\"");
	strcat(cmd_buffer, port_buff);
	strcat(cmd_buffer, "\"\r\n\0");

	int32_t rc=-1;
	Timer timer;
	TimerInit(&timer);
	at_modem_flush_rx_buffer(p_mdm);
	p_mdm->send((char*)p_mdm->tx_cmd);
	TimerCountdownMS(&timer,5000);
	while ((at_modem_search_response_string(p_mdm,"ALREADY CONNECT\r\n") < 0)
	               &&(at_modem_search_response_string(p_mdm,"CONNECT OK\r\n") < 0)
	                && !TimerIsExpired(&timer)) {

	        at_modem_read_all(p_mdm);
	}
	if(TimerIsExpired(&timer)){
	        rc=-1;
	}else{
	        rc=0;
	}
	timer_free(&timer);
	return rc;
}

int32_t sim800a_tcp_shutdown(Sim800a* p_sim){
	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char *cmd_buffer = (char*) p_mdm->tx_cmd;
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	char *str = (char*) AT_CMD_TCP_SHUTDOWN;
	while (*str) {
		*cmd_buffer++ = *str++;
	}

	*cmd_buffer++ = '\r';
	*cmd_buffer++ = '\n';
	*cmd_buffer++ = '\0';
	return at_modem_send_cmd(p_mdm,"SHUT OK\r\n",1000);
}

int32_t sim800a_terminate_data_sending(Sim800a* p_sim){
/* send terminate command */
        AT_Modem* p_mdm=(AT_Modem*)p_sim;
	p_mdm->tx_cmd[0]=0x1A;
	p_mdm->tx_cmd[1]='\0';
	return at_modem_send_cmd(p_mdm,"OK\r\n",3000);
}


int32_t sim800a_start_data_sending(Sim800a* p_sim){

        /* request send package  and switch to data mode */
	AT_Modem *p_mdm = (AT_Modem*)p_sim;
	char *cmd_buffer = (char*) p_mdm->tx_cmd;
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	char *str = (char*) AT_CMD_SEND_PACK;
	while (*str) {
		*cmd_buffer++ = *str++;
	}
	*cmd_buffer++ = '\r';
	*cmd_buffer++ = '\n';
	*cmd_buffer++ = '\0';

	return at_modem_send_cmd(p_mdm,"> ",2000);
}

void sim800a_close_tcp_connect(Sim800a *p_sim) {
	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char *cmd_buffer = (char*) p_mdm->tx_cmd + 3;
	char *str = (char*) AT_CMD_TCP_CLOSE;
	*cmd_buffer++ = '=';
	while (*str) {
		*cmd_buffer++ = *str++;
	}

	*cmd_buffer++ = '\r';
	*cmd_buffer++ = '\n';
	*cmd_buffer++ = '\0';
	at_modem_send_cmd(p_mdm,"OK\r\n",2000);
}

void sim800a_send_pack(Sim800a *p_sim, const char *s) {

	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	char *cmd_buffer = (char*) (p_mdm->tx_cmd + 3);
	char *str = (char*) AT_CMD_SEND_PACK;
	while (*str) {
		*cmd_buffer++ = *str++;
	}
	*cmd_buffer++ = '=';
	while (*s) {
		*cmd_buffer++ = *s++;
	}
	*cmd_buffer++ = '\r';
	*cmd_buffer++ = '\n';
	*cmd_buffer++ = '\0';
	at_modem_send_cmd(p_mdm,"OK\r\n",3000);
}

int32_t sim800a_send_pack_len(Sim800a *p_sim, const char *s, const int32_t len) {

	memcpy(p_sim->base.tx_cmd,s,len);
	p_sim->base.tx_cmd[len]=0;
	p_sim->base.send_len((const char*)p_sim->base.tx_cmd,len);
	return len;
}

static int32_t sim800a_is_state_response(Sim800a* p_sim){

        AT_Modem* p_mdm=(AT_Modem*) p_sim;
 	if(at_modem_search_response_string(p_mdm,"IP INITIAL\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_IP_INITIAL;
	        return 0;
	}

	if(at_modem_search_response_string(p_mdm,"IP START\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_IP_START;
	        return 1;
	}


	if(at_modem_search_response_string(p_mdm,"IP CONFIG\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_IP_CONFIG;
	        return 2;
	}

	if(at_modem_search_response_string(p_mdm,"IP GPRSACT\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_IP_GPRSACT;
	        return 3;
	}

	if(at_modem_search_response_string(p_mdm,"IP STATUS\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_IP_STATUS;
	        return 4;
	}

	if(at_modem_search_response_string(p_mdm,"TCP CONNECTING\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_TCP_CONNECTING;
	        return 5;
	}


	if(at_modem_search_response_string(p_mdm,"CONNECT OK\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_CONNECT_OK;
	        return 6;
	}

	if(at_modem_search_response_string(p_mdm,"ALREADY CONNECT\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_CONNECT_OK;
	        return 6;
	}

	if(at_modem_search_response_string(p_mdm,"TCP CLOSING\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_CLOSING;
	        return 7;
	}

	if(at_modem_search_response_string(p_mdm,"TCP CLOSED\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_CLOSED;
	        return 8;
	}

	if(at_modem_search_response_string(p_mdm,"PDP DEACT\r\n")>=0){
	        p_sim->client_state=SIM800_CLIENT_ST_PDP_DEACT;
	        return 9;
	}
	return -1;
}

/*
 * 0 IP INITIAL
 * 1 IP START
 * 2 IP CONFIG
 * 3 IP GPRSACT
 * 4 IP STATUS
 * 5 TCP CONNECTING/UDP CONNECTING/SERVER LISTENING
 * 6 CONNECT OK
 * 7 TCP CLOSING/UDP CLOSING
 * 8 TCP CLOSED/UDP CLOSED
 * 9 PDP DEACT
 */
int32_t sim800a_update_client_state(Sim800a* p_sim){

	AT_Modem *p_mdm = (AT_Modem*) p_sim;
	int32_t rc=-1;
	char *cmd_buffer = (char*) p_mdm->tx_cmd;
	*cmd_buffer++='A';
	*cmd_buffer++='T';
	*cmd_buffer++='+';
	char *str = (char*) AT_CMD_GET_CONNECTION_STATUS;
	while (*str) {
		*cmd_buffer++ = *str++;
	}

	*cmd_buffer++ = '\r';
	*cmd_buffer++ = '\n';
	*cmd_buffer++ = '\0';

	Timer timer;
	TimerInit(&timer);
	at_modem_flush_rx_buffer(p_mdm);
	p_mdm->send((char*)p_mdm->tx_cmd);
	TimerCountdownMS(&timer,3000);
	while ((sim800a_is_state_response(p_sim)<0)&& !TimerIsExpired(&timer)) {
	        at_modem_read_all(p_mdm);
	}
	if(TimerIsExpired(&timer)){
	        rc=-1;
	}
	else{
	        rc=0;
	}

	timer_free(&timer);
	return rc;
}

