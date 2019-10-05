	#include <stdio.h>
	#include <stdlib.h>

	#include "lista.h"

	/* void allocate(p_no list, int size) {
		p_no head = list->next;
		p_no first = head->next;
		
		int position = 0;
		int counter = 0;
		int savedPosition = 0;
		
		int savedPositionWithHigherCounter = 0;
		int savedCounter = 0;

		for (p_no actual = first; actual != head; actual = actual->next) {		
			if (actual->used == 0) {
				counter = counter + 1;
			} else {
				if (counter >= size) {
					if (savedCounter > size && counter < savedCounter) {
						savedPositionWithHigherCounter = savedPosition;
						savedCounter = counter;
					} else if (savedCounter < size) {
						savedPositionWithHigherCounter = savedPosition;
						savedCounter = counter;
					}
				}
				savedPosition = position + 1;
				counter = 0;
			}
			
			position = position + 1;
		}
		
		if (counter >= size) {
					if (savedCounter > size && counter < savedCounter) {
						savedCounter = counter;
					} else if (savedCounter < size) {
						savedPositionWithHigherCounter = savedPosition;
						savedCounter = counter;
					}
				}
		
		if (savedCounter >= size) {
			p_no actual = first;
			for (int i = 1; i <= savedPositionWithHigherCounter; i++) {
				actual = actual->next;
			}
			for (int i = 1; i <= size; i++) {
				actual->used=1;
				actual = actual->next;
			}
		}
	}

	int haveEnoughSpace(p_no list, int size, int position) {
		p_no head = list->next;
		p_no first = head->next;
		p_no actual = first;
		for (int i = 1; i <= position; i++) {
			actual = actual->next;
		}
		for (int i = 1; i <= size; i++) {
			if (actual->used == 1 || actual->used == -1) {
				return 0;		
			}
			actual = actual->next;
		}
		return 1;
	}

	void allocateAtPosition(p_no list, int size, int position) {
		p_no head = list->next;
		p_no first = head->next;
		p_no actual = first;
		for (int i = 1; i <= position; i++) {
			actual = actual->next;
		}
		for (int i = 1; i <= size; i++) {
			actual->used=1;
			actual = actual->next;
		}
	}
*/
	p_no disallocate(p_no list, int initial_position, int size) {
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
					//add new before actual node
					newNo->next = actual;
					newNo->prev = actual->prev;
					actual->prev->next = newNo;
					actual->prev = newNo;
				} else if (actual->next == head) {
					newNo->next = head;
					newNo->prev = list;
					list->next = newNo;
					head->prev = newNo;
					list = newNo;
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
					//save data
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
		/*
		for (int i = 1; i <= n - 1; i++) {
			list = add_no(list);
		}
		//p_no head = list->next;
		//p_no first = head->next;

		for (p_no actual = first; actual != head; actual = actual->next) {
			printf("%d\n", actual->used);
		}*/
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
				//disallocate(list, initial_position, size_before);
				//if (haveEnoughSpace(list, size_after, initial_position)) {
				//	allocateAtPosition(list, size_after, initial_position);			
				//} else {
				//	allocate(list, size_after);
				//}
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
