/*
 * linked_list.h
 *
 *  Created on: Mar 30, 2021
 *      Author: KhanhDinh
 */

#ifndef SERVICE_LINKED_LIST_LINKED_LIST_H_
#define SERVICE_LINKED_LIST_LINKED_LIST_H_

#include "stdint.h"
#include "stdlib.h"

#define END_OF_LIST	0

typedef struct NODE NODE;
struct NODE{
	uint8_t data;
	struct NODE* p_next;
};

typedef struct LIST LIST;
struct LIST{
	NODE* p_head;
	NODE* p_tail;
	uint8_t cnt;
	NODE* p_temp;
};

NODE* node_construct(uint8_t data);
LIST* list_init(void);
void list_insert_to_tail(LIST* p_list, uint8_t data);
void list_remove_node(LIST* p_list, uint8_t data);
uint8_t list_walk_down(LIST* p_list);

#endif /* SERVICE_LINKED_LIST_LINKED_LIST_H_ */
