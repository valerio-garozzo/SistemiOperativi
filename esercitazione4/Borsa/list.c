#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Crea un nuovo nodo */
NODE* createNode(Titolo item) {
    NODE* p = (NODE*)malloc(sizeof(NODE));
    assert(p != NULL);
    p->item = item;
    p->next = NULL;
    return p;
}

/* Inizializza una lista vuota */
LIST NewList() {
    return NULL;
}

/* Cancella la lista */
LIST DeleteList(LIST l) {
    while (!isEmpty(l)) {
        NODE* todel = l;
        l = l->next;
        free(todel);
    }
    return NULL;
}

/* Controlla se la lista è vuota */
BOOL isEmpty(LIST l) {
    return l == NULL;
}

/* Trova un titolo per nome */
Titolo* Find(LIST l, char* nome_agente) {
    while (l != NULL) {
        if (strcmp(l->item.nome_agente, nome_agente) == 0)
            return &(l->item);
        l = l->next;
    }
    return NULL;
}

/* Inserisce un titolo in ordine alfabetico */
LIST EnqueueOrdered(LIST l, Titolo item) {
    NODE* new_node = createNode(item);
    if (isEmpty(l) || strcmp(item.nome_agente, l->item.nome_agente) < 0) {
        new_node->next = l;
        return new_node;
    }
    LIST tmp = l;
    while (tmp->next != NULL && strcmp(tmp->next->item.nome_agente, item.nome_agente) < 0)
        tmp = tmp->next;
    
    new_node->next = tmp->next;
    tmp->next = new_node;
    return l;
}

/* Rimuove un titolo dalla lista */
LIST Dequeue(LIST l, char* nome_agente) {
    if (isEmpty(l)) return l;
    if (strcmp(l->item.nome_agente, nome_agente) == 0) {
        NODE* todel = l;
        l = l->next;
        free(todel);
        return l;
    }
    LIST tmp = l;
    while (tmp->next != NULL && strcmp(tmp->next->item.nome_agente, nome_agente) != 0)
        tmp = tmp->next;
    
    if (tmp->next != NULL) {
        NODE* todel = tmp->next;
        tmp->next = tmp->next->next;
        free(todel);
    }
    return l;
}

/* Stampa la lista */
void PrintList(LIST l) {
    while (!isEmpty(l)) {
        printf("Agente: %s, Quantità: %d, Prezzo: %d, Prezzo Minimo: %d\n",
               l->item.nome_agente, l->item.quantità, l->item.prezzo_corrente, l->item.prezzo_minimo);
        l = l->next;
    }
}
