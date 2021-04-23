/*
 * cabinet_door.c
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_door.h"

Cabinet_door* door_construct(void){
	Cabinet_door* p_door = (Cabinet_door*)malloc(sizeof(Cabinet_door));
	while(p_door == NULL);
	p_door->state = DOOR_ST_CLOSE;
	p_door->io_state = ios_construct();
	p_door->solenoid = sw_construct();
	return p_door;
}
