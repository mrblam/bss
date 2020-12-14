/*
 * sim800a.h
 *
 *  Created on: Oct 6, 2020
 *      Author: quangnd
 */

#ifndef COMPONENT_SIM800A_SIM800A_H_
#define COMPONENT_SIM800A_SIM800A_H_

#include "sim800a_hal.h"
#include "at_modem.h"

typedef struct Sim800a_t Sim800a;
typedef enum SIM800_TRANSFER_MODE_t{
	SIM800_TRANSFER_CMD_MODE=1,
	SIM800_TRANSFER_TRANSPARENT_MODE,

}SIM800_TRANSFER_MODE;

#define AT_CMD_CONNECT_GPRS 					"CGATT"
#define AT_CMD_TCP_SHUTDOWN                                     "CIPSHUT"
#define AT_CMD_SET_APN                                          "CSTT"
#define AT_CMD_ACTIVE_GPRS 					"CIICR"
#define AT_CMD_GET_LOCAL_IP					"CIFSR"

#define AT_CMD_TCP_CONNECT					"CIPSTART"
#define AT_CMD_TCP_CLOSE					"CIPCLOSE"
#define AT_CMD_GET_CONNECTION_STATUS 		                "CIPSTATUS"
#define AT_CMD_SEND_PACK	 				"CIPSEND"

typedef enum SIM800_CLIENT_STATE_t{
       SIM800_CLIENT_ST_IP_INITIAL=0,
       SIM800_CLIENT_ST_IP_START,
       SIM800_CLIENT_ST_IP_CONFIG,
       SIM800_CLIENT_ST_IP_GPRSACT,
       SIM800_CLIENT_ST_IP_STATUS,
       SIM800_CLIENT_ST_TCP_CONNECTING,
       SIM800_CLIENT_ST_CONNECT_OK,
       SIM800_CLIENT_ST_CLOSING,
       SIM800_CLIENT_ST_CLOSED,
       SIM800_CLIENT_ST_PDP_DEACT,
       SIM800_CLIENT_ST_PDP_ERROR,
       SIM800_CLIENT_ST_PDP_SETUP

}SIM800_CLIENT_STATE;

struct Sim800a_t{
	AT_Modem base;
	SIM800_CLIENT_STATE client_state;
};

void sim800a_init(Sim800a* p_sim);
int32_t sim800a_set_transfer_mode(Sim800a* p_sim,SIM800_TRANSFER_MODE mode);
int32_t sim800a_connect_gprs(Sim800a* p_sim);
int32_t sim800a_disconnect_gprs(Sim800a* p_sim);
int32_t sim800a_set_apn(Sim800a* p_sim,const char*, const char*, const char*);
int32_t sim800a_active_gprs(Sim800a* p_sim);

int32_t sim800a_connect_tcp_server(Sim800a* p_sim,const char*,const uint32_t);
void sim800a_close_tcp_connect(Sim800a*);
int32_t sim800a_get_local_ip(Sim800a* p_sim,char* ip);
void sim800a_send_pack(Sim800a* p_sim,const char* s);
int32_t sim800a_terminate_data_sending(Sim800a* p_sim);
int32_t sim800a_start_data_sending(Sim800a* p_sim);
int32_t sim800a_send_pack_len(Sim800a* p_sim,const char* s,const int32_t len);
uint8_t sim800a_is_active(Sim800a* p_sim);
int32_t sim800a_turnoff_echo(Sim800a* p_sim);
int32_t sim800a_tcp_shutdown(Sim800a* p_sim);
int32_t sim800a_update_client_state(Sim800a* p_sim);

#endif /* COMPONENT_SIM800A_SIM800A_H_ */
