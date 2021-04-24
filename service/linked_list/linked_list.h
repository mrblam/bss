/*
 * linked_list.h
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#ifndef SERVICE_LINKED_LIST_LINKED_LIST_H_
#define SERVICE_LINKED_LIST_LINKED_LIST_H_

#include "stdio.h"
#include "cabinet_cell.h"
#include "stdint.h"
#include "stdlib.h"
#include "malloc.h"

#define END_OF_LIST	0

typedef struct Cabinet_Node_t Cabinet_Node;
typedef struct Cabinet_List_t Cabinet_List;

struct Cabinet_Node_t{
	Cabinet* data;
	Cabinet_Node* p_next;
};

struct Cabinet_List_t{
	Cabinet_Node* p_head;
	Cabinet_Node* p_tail;
	uint8_t cnt;
	Cabinet_Node* p_temp;
};

Cabinet_Node* cab_node_construct(Cabinet* p_cc);
Cabinet_List* cab_list_init(void);
void cab_list_insert_to_tail(Cabinet_List* p_list, Cabinet* p_cc);
void cab_list_remove_node(Cabinet_List* p_list, Cabinet* p_cc);
Cabinet_Node* cab_list_walk_down(Cabinet_List* p_list);
void cab_list_export_data(Cabinet_List* p_list);
void cab_list_reset_temp(Cabinet_List* p_list);

#endif /* SERVICE_LINKED_LIST_LINKED_LIST_H_ */
