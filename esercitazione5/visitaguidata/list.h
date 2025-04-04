#ifndef _LIST_H
#define _LIST_H

#define BOOL int
#define FALSE 0
#define TRUE (!FALSE)

#define NAME_LEN 30

// Elemento base contenuto nella lista
typedef struct {
    char name[NAME_LEN];
    int min_visitors;
    int max_visitors;
    int current_visitors;
    int guide_socket;
} Guide;

struct LINKED_LIST_NODE {
    Guide item;
    struct LINKED_LIST_NODE* next;
};

typedef struct LINKED_LIST_NODE NODE;
typedef NODE* LIST;

LIST NewList();
LIST DeleteList(LIST l);
BOOL isEmpty(LIST l);
int getLength(LIST l);
Guide getHead(LIST l);
LIST EnqueueLast(LIST l, Guide item);
LIST DequeueGuideBySocket(LIST l, int socket_fd);
Guide* FindSuitableGuide(LIST l, int group_size);

#endif





