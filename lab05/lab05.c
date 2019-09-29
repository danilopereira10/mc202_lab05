#include <stdio.h>
#include <stdlib.h>

#include "lista.h"

void allocate(p_no list, int size) {
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
			savedPosition = position + 1;
			if (counter > savedCounter) {
				savedPositionWithHigherCounter = savedPosition;
				savedCounter = counter;
			}
			counter = 0;
		}
		
		position = position + 1;
	}
	
	if (counter > savedCounter) {
		savedPositionWithHigherCounter = savedPosition;
		savedCounter = counter;
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

void disallocate(p_no list, int initial_position, int size) {
	p_no head = list->next;
	p_no first = head->next;
	p_no actual = first;
	for (int i = 1; i <= initial_position; i++) {
		actual = actual->next;
	}
	for (int i = 1; i <= size; i++) {
		actual->used = 0;
		actual = actual->next;
	}
}

void print_list(p_no list) {
	p_no head = list->next;
	p_no first = head->next;
	
	int counter = 0;
	int position = 0;
	
	int savedPosition = 0;
	
	for (p_no actual = first; actual != head; actual = actual->next) {
		if (actual->used == 0) {
			counter = counter + 1;
		} else {
			if (counter > 0) {
				printf("%d %d\n", savedPosition, counter);
				counter = 0;
			}
			savedPosition = position + 1;
		}
		position = position + 1;
	}
	
	if (counter > 0) {
		printf("%d %d\n", savedPosition, counter);
	}
}

int main() {
	int m, n;
	char discardWhiteSpace;
	char option;
	p_no list;
	scanf("%d %d", &m, &n);

	list = create_list();
	
	for (int i = 1; i <= n - 1; i++) {
		list = add_no(list);
	}
	//p_no head = list->next;
	//p_no first = head->next;

	/*for (p_no actual = first; actual != head; actual = actual->next) {
		printf("%d\n", actual->used);
	}*/
	
	for (int i = 1; i <= m; i++) {
		scanf("%c", &discardWhiteSpace);
		scanf("%c", &option);
		if (option == 'A') {
			int size;
			scanf("%d", &size);
			allocate(list, size);
		} else if (option == 'D') {
			int initial_position, size;
			scanf("%d", &initial_position);			
			scanf("%d", &size);
			disallocate(list, initial_position, size);
		} else if (option == 'R') {
			int initial_position, size_before, size_after;
			scanf("%d", &initial_position);			
			scanf("%d", &size_before);
			scanf("%d", &size_after);
		} else if (option == 'P') {
			printf("heap:\n");
			print_list(list);
		}
	}
	
	printf("\n");

	return EXIT_SUCCESS;
}

