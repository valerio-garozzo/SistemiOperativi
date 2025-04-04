#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

NODE* createNode(Guide item) {
    NODE* node = malloc(sizeof(NODE));
    node->item = item;
    node->next = NULL;
    return node;
}

LIST NewList() { return NULL; }

BOOL isEmpty(LIST l) { return l == NULL; }

LIST DeleteList(LIST l) {
    while (l) {
        NODE* tmp = l;
        l = l->next;
        free(tmp);
    }
    return NULL;
}

int getLength(LIST l) {
    int count = 0;
    while (l) {
        count++;
        l = l->next;
    }
    return count;
}

Guide getHead(LIST l) {
    assert(!isEmpty(l));
    return l->item;
}

LIST EnqueueLast(LIST l, Guide item) {
    NODE* node = createNode(item);
    if (!l) return node;

    NODE* tmp = l;
    while (tmp->next) tmp = tmp->next;
    tmp->next = node;
    return l;
}

LIST DequeueGuideBySocket(LIST l, int socket_fd) {
    if (!l) return NULL;

    if (l->item.guide_socket == socket_fd) {
        NODE* to_del = l;
        l = l->next;
        free(to_del);
        return l;
    }

    NODE* prev = l;
    NODE* curr = l->next;
    while (curr) {
        if (curr->item.guide_socket == socket_fd) {
            prev->next = curr->next;
            free(curr);
            return l;
        }
        prev = curr;
        curr = curr->next;
    }
    return l;
}

Guide* FindSuitableGuide(LIST l, int group_size) {
    Guide* best = NULL;
    while (l) {
        int remaining = l->item.max_visitors - l->item.current_visitors;
        if (remaining >= group_size) {
            if (!best || remaining < (best->max_visitors - best->current_visitors))
                best = &l->item;
        }
        l = l->next;
    }
    return best;
}
