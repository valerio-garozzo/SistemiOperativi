#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>

#define TITOLO_LENGTH 20  

/* Definizione di BOOL per maggiore chiarezza */
typedef enum { FALSE = 0, TRUE = 1 } BOOL;

/*** Elemento base contenuto nella lista ***/
typedef struct {
    char titolo[TITOLO_LENGTH + 1];  // +1 per terminatore null
    int num_copie;
    BOOL tipo;
} ItemType;

/* Nodo della lista */
struct LINKED_LIST_NODE {
    ItemType item;
    struct LINKED_LIST_NODE* next;
};

/* Alias per rendere il codice più leggibile */
typedef struct LINKED_LIST_NODE NODE;
typedef NODE* LIST;

/* Definizione della funzione di confronto tra due elementi */
int itemCompare(ItemType item1, ItemType item2);

/*** Costruttori/Distruttori ***/

/* Inizializza una nuova lista */
LIST NewList();

/* Dealloca tutti gli elementi della lista */
LIST DeleteList(LIST l);

/*** Predicati ***/

/* Ritorna TRUE se la lista è vuota */
BOOL isEmpty(LIST l);

/* Ritorna il numero di elementi presenti nella lista */
int getLength(LIST l);

/*** Selettori ***/

/* Restituisce l'elemento in testa alla lista */
ItemType getHead(LIST l);

/* Restituisce l'elemento in coda alla lista */
ItemType getTail(LIST l);

/* Cerca un elemento nella lista e ne restituisce il puntatore, NULL se non trovato */
ItemType* Find(LIST l, ItemType item);

/*** Trasformatori ***/

/* Inserisce un elemento in testa alla lista */
LIST EnqueueFirst(LIST l, ItemType item);

/* Inserisce un elemento in coda alla lista */
LIST EnqueueLast(LIST l, ItemType item);

/* Inserisce un elemento mantenendo la lista ordinata */
LIST EnqueueOrdered(LIST l, ItemType item);

/* Rimuove il primo elemento della lista */
LIST DequeueFirst(LIST l);

/* Rimuove l'ultimo elemento della lista */
LIST DequeueLast(LIST l);

/* Rimuove un elemento specifico dalla lista */
LIST Dequeue(LIST l, ItemType item);

/*** Stampe ***/

/* Stampa un elemento */
void PrintItem(ItemType item);

/* Stampa l'intera lista */
void PrintList(LIST l);

#endif

