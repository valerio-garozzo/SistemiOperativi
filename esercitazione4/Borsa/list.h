#ifndef _LIST_H
#define _LIST_H

#define BOOL int
#define FALSE 0
#define TRUE (!FALSE)

/*** Struttura per rappresentare un titolo azionario ***/
typedef struct {
    char nome_agente[30];
    int quantità;
    int prezzo_corrente;
    int prezzo_minimo;
    int ricavo_totale;
} Titolo;

/* Nodo della lista */
struct LINKED_LIST_NODE {
    Titolo item;
    struct LINKED_LIST_NODE* next;
};

/* Lista come puntatore al primo nodo */
typedef struct LINKED_LIST_NODE NODE;
typedef NODE* LIST;

/*** Funzioni della lista ***/
LIST NewList();
LIST DeleteList(LIST l);
BOOL isEmpty(LIST l);
int getLength(LIST l);
Titolo* Find(LIST l, char* nome_agente);
LIST EnqueueOrdered(LIST l, Titolo item);
LIST Dequeue(LIST l, char* nome_agente);
void PrintList(LIST l);

#endif
