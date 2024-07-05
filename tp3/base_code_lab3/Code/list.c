/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;//DoubleLinkedlt dans le cours

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement* whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement* sentinel;
	int size; 
};


typedef struct s_SubList {//ok
	LinkedElement *head;
	LinkedElement *tail;
	
} SubList;

/*-----------------------------------------------------------------*/

List* list_create(void) {//base vu en td
	List* l = malloc(sizeof(struct s_List));
	l->sentinel = malloc(sizeof(LinkedElement));
	l->sentinel->previous = l->sentinel->next = l->sentinel;
	l->size = 0;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {//VU en td
	LinkedElement *e = malloc(sizeof(LinkedElement));
	e->value = v;

	e->next = l->sentinel;
	e->previous = e->next->previous;
	e->previous->next = e;
	e->next->previous = e;

	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList *l) {
   
    LinkedElement *e = (*l)->sentinel->next;

    while (e != (*l)->sentinel) {
        LinkedElement *next = e->next;
        free(e);
        e = next;
    }

    free((*l)->sentinel);
    free(*l);
    *l = NULL; 
}

/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
	LinkedElement *e = malloc(sizeof(LinkedElement));
	e->value = v;

	
	e->previous = l->sentinel;
	e->next = l->sentinel->next;
	e->next->previous = e;
	l->sentinel->next = e;

	

	++(l->size);
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	assert(!list_is_empty(l)); //voir  tad list
	
	return l->sentinel->next->value; //le next de sentinel est le premier element 
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	assert(!list_is_empty(l)); //voir  tad list
	
	return l->sentinel->previous->value; //previous de sentinel est le dernier element 
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) { //td
	assert(!list_is_empty(l)); //voir  tad list

	LinkedElement *e = NULL;
	e = l->sentinel->next;
	l->sentinel->next = e->next;
	l->sentinel->next->previous = l->sentinel;
	--(l->size);

	free(e);
	return l;

}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	assert(!list_is_empty(l)); //voir  tad list

	LinkedElement *e = NULL;
	e = l->sentinel->previous;
	e->previous->next = l->sentinel;
	l->sentinel->previous = e->previous;


	--(l->size);
	free(e);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
	assert((0 <= p) && (p <= l->size));
	LinkedElement *insert = l->sentinel->next;
	while (p--) insert = insert->next;
	LinkedElement *e = malloc(sizeof(LinkedElement)); e->value = v;
	e->next = insert; e->previous = insert->previous; e->previous->next = e; e->next->previous = e; ++(l->size);
	return l;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) { //diap43 
	assert((0 <= p) && (p < l->size)); 
	LinkedElement *remove = l->sentinel->next; 
	while (p--) remove = remove->next; 

    remove->previous->next = remove->next;
	remove->next->previous = remove->previous;
	free(remove);
	--(l->size);
	
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	assert(!list_is_empty(l) && (0 <= p) && p < l->size); 
	LinkedElement *elem = l->sentinel->next;
	for(int i=0; i<p; i++){
		elem = elem->next;
	}
	return elem->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) { 
	if(list_size(l) !=0){
		return false;
	}
	return true;
}

/*-----------------------------------------------------------------*/

int list_size(const List* l) { 
	return l->size;
}

/*-----------------------------------------------------------------*/

List* list_map(List* l, SimpleFunctor f) { //td
	for(LinkedElement *e = l->sentinel->next;  e!= l->sentinel;   e = e->next){
		e->value = f(e->value);
	}

	return l;
}


List* list_reduce(List* l, ReduceFunctor f, void *userData) { //td
	for(LinkedElement *e = l->sentinel->next; e!= l->sentinel; e = e->next){
		f(e->value,userData);
	}
	return l;
}

/*-----------------------------------------------------------------*/







/*qui d´ecoupe une liste l en deux
sous listes de tailles ´egales `a 1 ´el´ement pr`es. A partir de la liste doublement chaˆın´ee l
cette fonction renvoie une structure SubList dont le pointeur de tˆete d´esigne le dernier
´el´ement de la sous-liste gauche et le pointeur de queue le premier ´el´ement de la sous
liste droite*/

SubList list_split(SubList l) {
	while ((l.head)->next != l.tail){
		l.head = (l.head)->next;

		if ((l.head)->next != l.tail){
			l.tail = (l.tail)->previous;
		}
	}

	return l;
}

/*permettant de fusionner les deux listes tri´ees leftlist et rightlist

Licence Informatique
en respectant l’ordre d´efini par la fonction pass´ee en param`etre f*/
SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
	SubList mergedlist;

	if (f(leftlist.head->value, rightlist.head->value)){
		mergedlist.head = leftlist.head;
		leftlist.head = (leftlist.head)->next;
	}
	else
	{
		mergedlist.head = rightlist.head;
		rightlist.head = rightlist.head->next;
	}

	mergedlist.tail = mergedlist.head;

	while (leftlist.head != NULL && rightlist.head != NULL)
	{
		if (f(leftlist.head->value, rightlist.head->value)){
			mergedlist.tail->next = leftlist.head;
			leftlist.head = (leftlist.head)->next;
		}

		else{
			mergedlist.tail->next = rightlist.head;
			rightlist.head = (rightlist.head)->next;
		}

		mergedlist.tail->next->previous = mergedlist.tail;
		mergedlist.tail = (mergedlist.tail)->next;
	}

	if (leftlist.head != NULL){
		mergedlist.tail->next = leftlist.head;
	}

	else{
		mergedlist.tail->next = rightlist.head;
	}

	mergedlist.tail->next->previous = mergedlist.tail;

	while (mergedlist.tail->next != NULL){
		mergedlist.tail = (mergedlist.tail)->next;
	}

	return mergedlist;
}
/* Cette fonction devra ,dans un
premier temps, d´ecouper la liste l en deux sous liste (gauche et droite), puis effectuer
deux appels r´ecursif pour trier ces deux sous-listes et retourner le r´esultat de la fusion
de ces deux sous listes tri´ees. */

SubList list_mergesort(SubList l, OrderFunctor f) {

	if (l.head == l.tail){
		return l;
	}
	else{
		SubList leftPart;
		SubList rightPart;
		SubList splitList = list_split(l);

		leftPart.head = l.head;
		leftPart.tail = splitList.head;
		(leftPart.tail)->next = NULL;

		rightPart.head = splitList.tail;
		rightPart.tail = l.tail;
		(rightPart.tail)->next = NULL;

		return (list_merge(list_mergesort(leftPart, f), list_mergesort(rightPart, f), f));
	}
}
   

/* transformer la
liste avec sentinelle en une liste minimale correspondant `a votre structure SubList, trier
cette liste et transformer le r´esultat pour remettre en place la sentinelle.*/

List *list_sort(List *l, OrderFunctor f) {
	SubList sbList;

	sbList.head = (*l).sentinel->next;
	sbList.tail = (*l).sentinel->previous;
	sbList.head->previous = NULL;
	sbList.tail->next = NULL;

	sbList = list_mergesort(sbList, f);

	(*l).sentinel->next = sbList.head;
	(*l).sentinel->previous = sbList.tail;
	sbList.head->previous = (*l).sentinel; 
	sbList.tail->next = (*l).sentinel;

	return l;
}
