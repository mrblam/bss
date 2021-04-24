/*
 * linked_list.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "linked_list.h"

Cabinet_Node* cab_node_construct(Cabinet* p_cc){
	Cabinet_Node* p_node = (Cabinet_Node*)malloc(sizeof(Cabinet_Node));
	while(p_node == NULL);
	p_node->data = p_cc;
	p_node->p_next = NULL;
	return p_node;
}

Cabinet_List* cab_list_init(void){
	Cabinet_List* p_list = (Cabinet_List*)malloc(sizeof(Cabinet_List));
	while(p_list == NULL);
	p_list->p_head = NULL;
	p_list->p_tail = NULL;
	p_list->p_temp = NULL;
	p_list->cnt = 0;
	return p_list;
}

void cab_list_insert_to_head(Cabinet_List* p_list, Cabinet* p_cc){
	Cabinet_Node* p_node = cab_node_construct(p_cc);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_node->p_next = p_list->p_head;
		p_list->p_head = p_node;
	}
}

void cab_list_insert_to_tail(Cabinet_List* p_list, Cabinet* p_cc){
	Cabinet_Node* p_node = cab_node_construct(p_cc);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_list->p_tail->p_next = p_node;
		p_list->p_tail = p_node;
	}
}

void cab_list_remove_node(Cabinet_List* p_list, Cabinet* p_cc){
	Cabinet_Node **p_temp = &(p_list->p_head);
	while((*p_temp)->data != p_cc){
		p_temp = &((*p_temp)->p_next);
	}
	*p_temp = (*p_temp)->p_next;
}

uint8_t cab_list_find_data(Cabinet_List* p_list, Cabinet* p_cc){
	uint8_t pos = 1;
	Cabinet_Node **p_temp = &(p_list->p_head);
	while((*p_temp)->data != p_cc){
		p_temp = &((*p_temp)->p_next);
		pos++;
	}
	return pos;
}

Cabinet_Node* cab_list_walk_down(Cabinet_List* p_list){
	if(p_list->cnt == 0){
		p_list->cnt++;
		p_list->p_temp = p_list->p_head;
		return p_list->p_temp;
	}
	else{
		while(p_list->p_temp->p_next != NULL){
			p_list->p_temp = p_list->p_temp->p_next;
			if(p_list->p_temp == NULL) break;
			return p_list->p_temp;
		}
	p_list->cnt = 0;
	return NULL;
	}
}

void cab_list_export_data(Cabinet_List* p_list){
	Cabinet_Node* p_temp;
	for(p_temp = p_list->p_head; p_temp != NULL; p_temp = p_temp->p_next){
		printf("%d ", p_temp->data->cab_id);
	}
}

void cab_list_reset_temp(Cabinet_List* p_list){
	p_list->p_temp = p_list->p_head;
}
