#include <stdlib.h>

#include "lista.h"

p_no create_list() {
	p_no head = malloc(sizeof(p_no));
	p_no newList = malloc(sizeof(p_no));
	
	head->next = newList;
	head->prev = newList;
	head->used = -1;
	
	newList->next = head;
	newList->prev = head;
	newList->used = 0;
	return newList;
}

p_no add_no(p_no list) {
	p_no newNo = malloc(sizeof(p_no));
	newNo->next = list->next;
	newNo->prev = list;
	newNo->used = 0;
	list->next = newNo;
	return newNo;
}
