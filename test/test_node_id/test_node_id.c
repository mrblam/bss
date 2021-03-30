/*
 * test_node_id.c
 *
 *  Created on: Mar 30, 2021
 *      Author: KhanhDinh
 */

#include "cabinet_app.h"

Cabinet_App selex_bss;

int main(void){
	printf("selex motors");
	//the_app = ca_construct();
	ca_init(&selex_bss);

	ca_accept_assign_id(&selex_bss);
	ca_process_assign_id(&selex_bss);
	while(1){

	}
}
