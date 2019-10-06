#ifndef LISTA_H
#define LISTA_H

typedef struct No {
	struct No *next;
	struct No *prev;
	int initialPosition;
	int freeSpace;
} No;

typedef struct No * p_no;

p_no create_list();


#endif
