#include <stdio.h>
#include <stdlib.h>

#include "lista.h"

p_no disallocate(p_no list, int initial_position, int size) {
	if (size == 0) {
		return list;
	}
	p_no newNo = malloc(sizeof(p_no));
	p_no head = list->next;
	newNo->initialPosition = initial_position;
	newNo->freeSpace = size;
	if (list == head) { //add newNo to the end of the list
		newNo->next = head;
		newNo->prev = list;
		list->next = newNo;
		head->prev = newNo;
		list = newNo;
	} else {
		for (p_no actual = head->next; actual != head; actual = actual->next) {
			if (newNo->initialPosition < actual->initialPosition) {
				//add new node before actual node
				newNo->next = actual;
				newNo->prev = actual->prev;
				actual->prev->next = newNo;
				actual->prev = newNo;
				if (newNo->prev != head && newNo->prev->initialPosition + newNo->prev->freeSpace == newNo->initialPosition) {
					//unite new node with previous node
					newNo->prev->freeSpace = newNo->prev->freeSpace + newNo->freeSpace;
					newNo->prev->next = newNo->next;
					newNo->next->prev = newNo->prev;
					p_no previousNo = newNo->prev;
					if (list == newNo) {
						list = newNo->prev;
					}
					free(newNo);
					newNo = previousNo;
				}
				if (newNo->initialPosition + newNo->freeSpace == actual->initialPosition) {
					//unite new node with next node
					newNo->freeSpace = newNo->freeSpace + actual->freeSpace;
					newNo->next = actual->next;
					actual->next->prev = actual->prev;
					if (list == actual) {
						list = newNo;
					}
					free(actual);
					
				}
				return list;
			} else if (actual->next == head) {
				//add new node to the end of list
				newNo->next = head;
				newNo->prev = list;
				list->next = newNo;
				head->prev = newNo;
				list = newNo;
				if (actual->initialPosition + actual->freeSpace == newNo->initialPosition) {
					//unite last node with previous node
					actual->freeSpace = actual->freeSpace + newNo->freeSpace;
					actual->next = newNo->next;
					newNo->next->prev = newNo->prev;
					if (list == newNo) {
						list = actual;
					}
					free(newNo);
					
				}
				return list;
			}
		}
	}
	return list;	
}

p_no allocateAtPosition(p_no list, int position, int size) {
	if (size == 0) { //do nothing
		return list;
	}
	
	p_no head = list->next;
	p_no first = head->next;
	p_no choosenPosition = NULL;
	
	//this finds interval with position for allocating
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->initialPosition <= position && actual->initialPosition + actual->freeSpace - 1 >= position) {
			if(!(actual->freeSpace >= size)) { //space not enuff;
				return NULL;
			} else {
				//allocate effectively
				int initialChoosenPosition = choosenPosition->initialPosition;
				choosenPosition->freeSpace = choosenPosition->freeSpace - size - (position - choosenPosition->initialPosition);
				choosenPosition->initialPosition = choosenPosition->initialPosition + size + (position - choosenPosition->initialPosition);
				if (choosenPosition->freeSpace == 0) { 
					choosenPosition->prev->next = choosenPosition->next;
					choosenPosition->next->prev = choosenPosition->prev;
					free(choosenPosition);
					list = head->prev; //go to the last item of the list;
				}
				list = disallocate(list, initialChoosenPosition, position - initialChoosenPosition);
				return list;
			}
		}
	}
	return list;
}

p_no allocate(p_no list, int size) {
	if (size == 0) { //do nothing
		return list;
	}
	
	p_no head = list->next;
	p_no first = head->next;
	p_no choosenPosition = NULL;
	
	//this for chooses position for allocating
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->freeSpace >= size) {
			if (choosenPosition != NULL) { //check if there is a choosenPosition
				if (actual->freeSpace < choosenPosition->freeSpace) { //checks if this position is lower than the last one
					//save data
					choosenPosition = actual;
				}	
			} else {
				choosenPosition = actual;
			} 
		}
	}
	if (choosenPosition != NULL) {
		//allocate effectively
		choosenPosition->freeSpace = choosenPosition->freeSpace - size;
		choosenPosition->initialPosition = choosenPosition->initialPosition + size;
		if (choosenPosition->freeSpace == 0) {
			choosenPosition->prev->next = choosenPosition->next;
			choosenPosition->next->prev = choosenPosition->prev;
			free(choosenPosition);
			list = head->prev; //go to the last item of the list;
		}
	}
	return list;
}

p_no reallocate(p_no list, int initial_position, int size_before, int size_after) {
	p_no newList = disallocate(list, initial_position, size_before);
	p_no listAfterAllocation = allocateAtPosition(newList, initial_position, size_after);
	if (listAfterAllocation == NULL) {
		listAfterAllocation = allocate(newList, size_after);
	}
	return listAfterAllocation;
}

void print_list(p_no list) {
	p_no head = list->next;
	p_no first = head->next;
	
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->freeSpace > 0) {
			printf("\n%d %d", actual->initialPosition, actual->freeSpace);
		}
	}
}

int main() {
	int m, n;
	char discardWhiteSpace;
	char option;
	p_no list;
	scanf("%d %d", &m, &n);

	list = create_list(n);

	int firstPrint = 1;	

	for (int i = 1; i <= m; i++) {
		scanf("%c", &discardWhiteSpace);
		scanf("%c", &option);
		if (option == 'A') {
			int size;
			scanf("%d", &size);
			list = allocate(list, size);
		} else if (option == 'D') {
			int initial_position, size;
			scanf("%d", &initial_position);			
			scanf("%d", &size);
			list = disallocate(list, initial_position, size);
		} else if (option == 'R') {
			int initial_position, size_before, size_after;
			scanf("%d", &initial_position);			
			scanf("%d", &size_before);
			scanf("%d", &size_after);
			reallocate(list, initial_position, size_after, initial_position);
		} else if (option == 'P') {
			if (!firstPrint) {
				printf("\nheap:");
				print_list(list);
			} else {
				printf("heap:");
				print_list(list);
				firstPrint = 0;
			}
		}
	}
	
	printf("\n");

	return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>

#include "lista.h"

p_no disallocate(p_no list, int initial_position, int size) {
	if (size == 0) {
		return list;
	}
	p_no newNo = malloc(sizeof(p_no));
	p_no head = list->next;
	newNo->initialPosition = initial_position;
	newNo->freeSpace = size;
	if (list == head) { //add newNo to the end of the list
		newNo->next = head;
		newNo->prev = list;
		list->next = newNo;
		head->prev = newNo;
		list = newNo;
	} else {
		for (p_no actual = head->next; actual != head; actual = actual->next) {
			if (newNo->initialPosition < actual->initialPosition) {
				//add new node before actual node
				newNo->next = actual;
				newNo->prev = actual->prev;
				actual->prev->next = newNo;
				actual->prev = newNo;
				if (newNo->prev != head && newNo->prev->initialPosition + newNo->prev->freeSpace == newNo->initialPosition) {
					//unite new node with previous node
					newNo->prev->freeSpace = newNo->prev->freeSpace + newNo->freeSpace;
					newNo->prev->next = newNo->next;
					newNo->next->prev = newNo->prev;
					p_no previousNo = newNo->prev;
					if (list == newNo) {
						list = newNo->prev;
					}
					free(newNo);
					newNo = previousNo;
				}
				if (newNo->initialPosition + newNo->freeSpace == actual->initialPosition) {
					//unite new node with next node
					newNo->freeSpace = newNo->freeSpace + actual->freeSpace;
					newNo->next = actual->next;
					actual->next->prev = actual->prev;
					if (list == actual) {
						list = newNo;
					}
					free(actual);
					
				}
				return list;
			} else if (actual->next == head) {
				//add new node to the end of list
				newNo->next = head;
				newNo->prev = list;
				list->next = newNo;
				head->prev = newNo;
				list = newNo;
				if (actual->initialPosition + actual->freeSpace == newNo->initialPosition) {
					//unite last node with previous node
					actual->freeSpace = actual->freeSpace + newNo->freeSpace;
					actual->next = newNo->next;
					newNo->next->prev = newNo->prev;
					if (list == newNo) {
						list = actual;
					}
					free(newNo);
					
				}
				return list;
			}
		}
	}
	return list;	
}

p_no allocateAtPosition(p_no list, int position, int size) {
	if (size == 0) { //do nothing
		return list;
	}
	
	p_no head = list->next;
	p_no first = head->next;
	p_no choosenPosition = NULL;
	
	//this finds interval with position for allocating
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->initialPosition <= position && actual->initialPosition + actual->freeSpace - 1 >= position) {
			if(!(actual->freeSpace >= size)) { //space not enuff;
				return NULL;
			} else {
				//allocate effectively
				int initialChoosenPosition = choosenPosition->initialPosition;
				choosenPosition->freeSpace = choosenPosition->freeSpace - size - (position - choosenPosition->initialPosition);
				choosenPosition->initialPosition = choosenPosition->initialPosition + size + (position - choosenPosition->initialPosition);
				if (choosenPosition->freeSpace == 0) { 
					choosenPosition->prev->next = choosenPosition->next;
					choosenPosition->next->prev = choosenPosition->prev;
					free(choosenPosition);
					list = head->prev; //go to the last item of the list;
				}
				list = disallocate(list, initialChoosenPosition, position - initialChoosenPosition);
				return list;
			}
		}
	}
	return list;
}

p_no allocate(p_no list, int size) {
	if (size == 0) { //do nothing
		return list;
	}
	
	p_no head = list->next;
	p_no first = head->next;
	p_no choosenPosition = NULL;
	
	//this for chooses position for allocating
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->freeSpace >= size) {
			if (choosenPosition != NULL) { //check if there is a choosenPosition
				if (actual->freeSpace < choosenPosition->freeSpace) { //checks if this position is lower than the last one
					//save data
					choosenPosition = actual;
				}	
			} else {
				choosenPosition = actual;
			} 
		}
	}
	if (choosenPosition != NULL) {
		//allocate effectively
		choosenPosition->freeSpace = choosenPosition->freeSpace - size;
		choosenPosition->initialPosition = choosenPosition->initialPosition + size;
		if (choosenPosition->freeSpace == 0) {
			choosenPosition->prev->next = choosenPosition->next;
			choosenPosition->next->prev = choosenPosition->prev;
			free(choosenPosition);
			list = head->prev; //go to the last item of the list;
		}
	}
	return list;
}

p_no reallocate(p_no list, int initial_position, int size_before, int size_after) {
	p_no newList = disallocate(list, initial_position, size_before);
	p_no listAfterAllocation = allocateAtPosition(newList, initial_position, size_after);
	if (listAfterAllocation == NULL) {
		listAfterAllocation = allocate(newList, size_after);
	}
	return listAfterAllocation;
}

void print_list(p_no list) {
	p_no head = list->next;
	p_no first = head->next;
	
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->freeSpace > 0) {
			printf("\n%d %d", actual->initialPosition, actual->freeSpace);
		}
	}
}

int main() {
	int m, n;
	char discardWhiteSpace;
	char option;
	p_no list;
	scanf("%d %d", &m, &n);

	list = create_list(n);

	int firstPrint = 1;	

	for (int i = 1; i <= m; i++) {
		scanf("%c", &discardWhiteSpace);
		scanf("%c", &option);
		if (option == 'A') {
			int size;
			scanf("%d", &size);
			list = allocate(list, size);
		} else if (option == 'D') {
			int initial_position, size;
			scanf("%d", &initial_position);			
			scanf("%d", &size);
			list = disallocate(list, initial_position, size);
		} else if (option == 'R') {
			int initial_position, size_before, size_after;
			scanf("%d", &initial_position);			
			scanf("%d", &size_before);
			scanf("%d", &size_after);
			reallocate(list, initial_position, size_after, initial_position);
		} else if (option == 'P') {
			if (!firstPrint) {
				printf("\nheap:");
				print_list(list);
			} else {
				printf("heap:");
				print_list(list);
				firstPrint = 0;
			}
		}
	}
	
	printf("\n");

	return EXIT_SUCCESS;
}
