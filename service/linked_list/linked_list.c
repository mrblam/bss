/*
 * linked_list.c
 *
 *  Created on: Mar 30, 2021
 *      Author: KhanhDinh
 */

#include "linked_list.h"

NODE* node_construct(uint8_t data){
	NODE* p_node = (NODE*)malloc(sizeof(NODE));
	while(p_node == NULL) return NULL;
	p_node->data = data;
	p_node->p_next = NULL;
	return p_node;
}

LIST* list_init(void){
	LIST* p_list = (LIST*)malloc(sizeof(LIST));
	while(p_list == NULL) return NULL;
	p_list->p_head = NULL;
	p_list->p_tail = NULL;
	p_list->p_temp = NULL;
	p_list->cnt = 0;
	return p_list;
}

void list_insert_to_head(LIST* p_list, uint8_t data){
	NODE* p_node = node_construct(data);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_node->p_next = p_list->p_head;
		p_list->p_head = p_node;
	}
}

void list_insert_to_tail(LIST* p_list, uint8_t data){
	NODE* p_node = node_construct(data);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_list->p_tail->p_next = p_node;
		p_list->p_tail = p_node;
	}
}

void list_remove_node(LIST* p_list, uint8_t data){
	NODE **p_temp = &(p_list->p_head);
	while((*p_temp)->data != data){
		p_temp = &((*p_temp)->p_next);
	}
	*p_temp = (*p_temp)->p_next;
}

uint8_t list_find_data(LIST* p_list, uint8_t data){
	uint8_t pos = 1;
	NODE **p_temp = &(p_list->p_head);
	while((*p_temp)->data != data){
		p_temp = &((*p_temp)->p_next);
		pos++;
	}
	return pos;
}

uint8_t list_walk_down(LIST* p_list){
	if(p_list->cnt == 0){
		p_list->cnt++;
		p_list->p_temp = p_list->p_head;
		return p_list->p_temp->data;
	}
	else{
		while(p_list->p_temp->p_next != NULL){
			p_list->p_temp = p_list->p_temp->p_next;
			return p_list->p_temp->data;
		}
	return 0; /* END OF LIST */
	}
}

