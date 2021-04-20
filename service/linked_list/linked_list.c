/*
 * linked_list.c
 *
 *  Created on: Apr 1, 2021
 *      Author: KhanhDinh
 */

#include "linked_list.h"

Cabinet_node* cab_node_construct(Cabinet_cell* p_cc){
	Cabinet_node* p_node = (Cabinet_node*)malloc(sizeof(Cabinet_node));
	while(p_node == NULL);
	p_node->data = p_cc;
	p_node->p_next = NULL;
	return p_node;
}

Cabinet_list* cab_list_init(void){
	Cabinet_list* p_list = (Cabinet_list*)malloc(sizeof(Cabinet_list));
	while(p_list == NULL);
	p_list->p_head = NULL;
	p_list->p_tail = NULL;
	p_list->p_temp = NULL;
	p_list->cnt = 0;
	return p_list;
}

void cab_list_insert_to_head(Cabinet_list* p_list, Cabinet_cell* p_cc){
	Cabinet_node* p_node = cab_node_construct(p_cc);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_node->p_next = p_list->p_head;
		p_list->p_head = p_node;
	}
}

void cab_list_insert_to_tail(Cabinet_list* p_list, Cabinet_cell* p_cc){
	Cabinet_node* p_node = cab_node_construct(p_cc);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_list->p_tail->p_next = p_node;
		p_list->p_tail = p_node;
	}
}

void cab_list_remove_node(Cabinet_list* p_list, Cabinet_cell* p_cc){
	Cabinet_node **p_temp = &(p_list->p_head);
	while((*p_temp)->data != p_cc){
		p_temp = &((*p_temp)->p_next);
	}
	*p_temp = (*p_temp)->p_next;
}

uint8_t cab_list_find_data(Cabinet_list* p_list, Cabinet_cell* p_cc){
	uint8_t pos = 1;
	Cabinet_node **p_temp = &(p_list->p_head);
	while((*p_temp)->data != p_cc){
		p_temp = &((*p_temp)->p_next);
		pos++;
	}
	return pos;
}

Cabinet_cell* cab_list_walk_down(Cabinet_list* p_list){
	if(p_list->cnt == 0){
		p_list->cnt++;
		p_list->p_temp = p_list->p_head;
		return p_list->p_temp->data;
	}
	else{
		while(p_list->p_temp->p_next != NULL){
			p_list->p_temp = p_list->p_temp->p_next;
			if(p_list->p_temp == NULL) break;
			return p_list->p_temp->data;
		}
	p_list->cnt = 0;
	return NULL; /* END OF LIST */
	}
}

void cab_list_export_data(Cabinet_list* p_list){
	Cabinet_node* p_temp;
	for(p_temp = p_list->p_head; p_temp != NULL; p_temp = p_temp->p_next){
		printf("%d ", p_temp->data->cab_id);
	}
}

/* ---------------------------------------------------------------------------*/
Node* node_construct(uint8_t data){
	Node* p_node = (Node*)malloc(sizeof(Node));
	while(p_node == NULL) return NULL;
	p_node->data = data;
	p_node->p_next = NULL;
	return p_node;
}

List* list_init(void){
	List* p_list = (List*)malloc(sizeof(List));
	while(p_list == NULL) return NULL;
	p_list->p_head = NULL;
	p_list->p_tail = NULL;
	p_list->p_temp = NULL;
	p_list->cnt = 0;
	return p_list;
}

void list_insert_to_head(List* p_list, uint8_t data){
	Node* p_node = node_construct(data);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_node->p_next = p_list->p_head;
		p_list->p_head = p_node;
	}
}

void list_insert_to_tail(List* p_list, uint8_t data){
	Node* p_node = node_construct(data);
	if(p_list->p_head == NULL){
		p_list->p_head = p_list->p_tail = p_node;
	}
	else{
		p_list->p_tail->p_next = p_node;
		p_list->p_tail = p_node;
	}
}

void list_remove_node(List* p_list, uint8_t data){
	Node **p_temp = &(p_list->p_head);
	while((*p_temp)->data != data){
		p_temp = &((*p_temp)->p_next);
	}
	*p_temp = (*p_temp)->p_next;
}

uint8_t list_find_data(List* p_list, uint8_t data){
	uint8_t pos = 1;
	Node **p_temp = &(p_list->p_head);
	while((*p_temp)->data != data){
		p_temp = &((*p_temp)->p_next);
		pos++;
	}
	return pos;
}

uint8_t list_walk_down(List* p_list){
	if(p_list->cnt == 0){
		p_list->cnt++;
		p_list->p_temp = p_list->p_head;
		return p_list->p_temp->data;
	}
	else{
		while(p_list->p_temp->p_next != NULL){
			p_list->p_temp = p_list->p_temp->p_next;
			if(p_list->p_temp == NULL) break;
			return p_list->p_temp->data;
		}
	p_list->cnt = 0;
	return END_OF_LIST; /* END OF LIST */
	}
}

void list_export_data(List* p_list){
	Node* p_temp;
	for(p_temp = p_list->p_head; p_temp != NULL; p_temp = p_temp->p_next){
		printf("%d ", p_temp->data);
	}
}

