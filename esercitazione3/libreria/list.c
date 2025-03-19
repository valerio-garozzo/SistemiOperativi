#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Inclusione necessaria
#include "list.h"

/************* Funzioni locali *********************************/

/* Alloca un nuovo nodo per la lista, copiandone all'interno l'elemento item */
NODE* createNode(ItemType item) {
    NODE* p = (NODE*)malloc(sizeof(NODE));
    assert(p != NULL);

    p->item = item;
    p->next = NULL;
    return p;
}

/* Dealloca il nodo p */
void deleteNode(NODE* p) {
    free(p);
}

/********** Funzioni standard *******************************/

/* Inizializza una nuova lista */
LIST NewList() {
    return NULL;
}

/* Azzera la lista */
LIST DeleteList(LIST l) {
    while (!isEmpty(l)) {
        NODE* todel = l;
        l = l->next;
        deleteNode(todel);
    }
    return NULL;
}

/* Ritorna TRUE se la lista è vuota */
BOOL isEmpty(LIST l) {
    return l == NULL;
}

/* Ritorna il numero di elementi presenti nella lista */
int getLength(LIST l) {
    int size = 0;
    LIST tmp = l;

    while (!isEmpty(tmp)) {
        ++size;
        tmp = tmp->next;
    }

    return size;
}

/* Restituisce l'elemento in testa alla lista, senza modificarla */
ItemType getHead(LIST l) {
    assert(!isEmpty(l));
    return l->item;
}

/* Restituisce l'elemento in coda alla lista, senza modificarla */
ItemType getTail(LIST l) {
    assert(!isEmpty(l));

    NODE* tmp = l;
    while (!isEmpty(tmp->next))
        tmp = tmp->next;

    return tmp->item;
}


int itemCompare(ItemType a, ItemType b) {
    return strcmp(a.titolo, b.titolo);
}

void PrintItem(ItemType item) {
    printf("%s\n", item.titolo);
}

/* Cerca l'elemento item nella lista e ne restituisce il puntatore, NULL se non trovato */
ItemType* Find(LIST l, ItemType item) {
    LIST tmp = l;
    while (tmp != NULL) {
        if (itemCompare(tmp->item, item) == 0) {
            return &(tmp->item);
        }
        tmp = tmp->next;
    }
    return NULL;
}

/* Inserisce un elemento nella prima posizione della lista */
LIST EnqueueFirst(LIST l, ItemType item) {
    NODE* new_node = createNode(item);
    new_node->next = l;
    return new_node;
}

/* Inserisce un elemento nell'ultima posizione della lista */
LIST EnqueueLast(LIST l, ItemType item) {
    NODE* new_node = createNode(item);

    if (isEmpty(l)) {
        return new_node;
    }

    LIST tmp = l;
    while (!isEmpty(tmp->next))
        tmp = tmp->next;

    tmp->next = new_node;
    return l;
}

/* Inserisce un elemento mantenendo la lista ordinata */
LIST EnqueueOrdered(LIST l, ItemType item) {
    NODE* new_node = createNode(item);

    if (isEmpty(l) || itemCompare(item, l->item) < 0) {
        new_node->next = l;
        return new_node;
    }

    LIST tmp = l;
    while (!isEmpty(tmp->next) && itemCompare(tmp->next->item, item) < 0) {
        tmp = tmp->next;
    }

    new_node->next = tmp->next;
    tmp->next = new_node;

    return l;
}

/* Toglie il primo elemento della lista */
LIST DequeueFirst(LIST l) {
    if (!isEmpty(l)) {
        NODE* todel = l;
        l = l->next;
        deleteNode(todel);
    }
    return l;
}

/* Toglie l'ultimo elemento della lista */
LIST DequeueLast(LIST l) {
    if (isEmpty(l)) return l;

    if (isEmpty(l->next)) {
        deleteNode(l);
        return NULL;
    }

    LIST tmp = l;
    while (!isEmpty(tmp->next->next))
        tmp = tmp->next;

    deleteNode(tmp->next);
    tmp->next = NULL;
    return l;
}

/* Toglie un elemento specifico dalla lista */
LIST Dequeue(LIST l, ItemType item) {
    if (isEmpty(l)) return l;

    if (itemCompare(l->item, item) == 0) {
        NODE* todel = l;
        l = l->next;
        deleteNode(todel);
        return l;
    }

    LIST tmp = l;
    while (!isEmpty(tmp->next) && itemCompare(tmp->next->item, item) != 0)
        tmp = tmp->next;

    if (!isEmpty(tmp->next)) {
        NODE* todel = tmp->next;
        tmp->next = tmp->next->next;
        deleteNode(todel);
    }

    return l;
}

/* Stampa a video la lista */
void PrintList(LIST l) {
    LIST tmp = l;
    while (!isEmpty(tmp)) {
        PrintItem(tmp->item);
        tmp = tmp->next;

        if (!isEmpty(tmp))
            printf(" ");
    }
}
