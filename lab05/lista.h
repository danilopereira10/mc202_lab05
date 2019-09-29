#ifndef LISTA_H
#define LISTA_H

typedef struct No {
	struct No *next;
	struct No *prev;
	int used;
} No;

typedef struct No * p_no;

p_no create_list();

p_no add_no(p_no list);

void imprimir_lista(p_no lista);


#endif
