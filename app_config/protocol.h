/*
 * protocol.h
 *
 *  Created on: Apr 8, 2021
 *      Author: KhanhDinh
 */

#ifndef APP_CONFIG_PROTOCOL_H_
#define APP_CONFIG_PROTOCOL_H_

/* CabinetControlApp & BSSMonitorApp Protocol
 * 		Slave <----------------> Master
 * Master Read: 	':' 'R' '<obj>' '<sub>' '<data>' '*'
 * Master Write:	':' 'W' '<obj>' '<sub>' '<data>' '*'
 */

#define CABINET	'C'


#endif /* APP_CONFIG_PROTOCOL_H_ */
