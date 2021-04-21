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

typedef struct Cabinet_node_t Cabinet_node;
typedef struct Cabinet_list_t Cabinet_list;

struct Cabinet_node_t{
	Cabinet_cell* data;
	Cabinet_node* p_next;
};

struct Cabinet_list_t{
	Cabinet_node* p_head;
	Cabinet_node* p_tail;
	uint8_t cnt;
	Cabinet_node* p_temp;
};

Cabinet_node* cab_node_construct(Cabinet_cell* p_cc);
Cabinet_list* cab_list_init(void);
void cab_list_insert_to_tail(Cabinet_list* p_list, Cabinet_cell* p_cc);
void cab_list_remove_node(Cabinet_list* p_list, Cabinet_cell* p_cc);
Cabinet_node* cab_list_walk_down(Cabinet_list* p_list);
void cab_list_export_data(Cabinet_list* p_list);
void cab_list_reset_temp(Cabinet_list* p_list);

/* -------------------------------------------------------------------------------- */
typedef struct Node_t Node;
typedef struct List_t List;

struct Node_t{
	uint8_t data;
	Node* p_next;
};

struct List_t{
	Node* p_head;
	Node* p_tail;
	Node* p_temp;
	uint8_t cnt;
};

Node* node_construct(uint8_t data);
List* list_init(void);
void list_insert_to_tail(List* p_list, uint8_t data);
void list_insert_to_head(List* p_list, uint8_t data);
void list_remove_node(List* p_list, uint8_t data);
uint8_t list_walk_down(List* p_list);
void list_export_data(List* p_list);
void list_reset_temp(List* p_list);

#endif /* SERVICE_LINKED_LIST_LINKED_LIST_H_ */
