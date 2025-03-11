#include <stdio.h>
#include <stdlib.h>

typedef int itemType;

typedef struct Node {
    itemType data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} LIST;

LIST EnqueueFirst(LIST l, itemType item) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = item;
    newNode->next = l.head;
    l.head = newNode;
    return l;
}

LIST DequeueLast(LIST l) {
    if (l.head == NULL) return l;
    if (l.head->next == NULL) {
        free(l.head);
        l.head = NULL;
        return l;
    }
    Node *temp = l.head;
    while (temp->next->next != NULL) {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    return l;
}

LIST EnqueueOrdered(LIST l, itemType item) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = item;
    newNode->next = NULL;
    if (l.head == NULL || l.head->data >= item) {
        newNode->next = l.head;
        l.head = newNode;
        return l;
    }
    Node *current = l.head;
    while (current->next != NULL && current->next->data < item) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    return l;
}

itemType *Find(LIST l, itemType item) {
    Node *current = l.head;
    while (current != NULL) {
        if (current->data == item) {
            return &(current->data);
        }
        current = current->next;
    }
    return NULL;
}

void PrintList(LIST l) {
    Node *temp = l.head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

/*
int main() {
    LIST myList = {NULL};
    
    myList = EnqueueFirst(myList, 7);
    myList = EnqueueFirst(myList, 4);
    PrintList(myList);
    
    myList = EnqueueOrdered(myList, 6);
    myList = EnqueueOrdered(myList, 2);
    PrintList(myList);
    
    myList = DequeueLast(myList);
    PrintList(myList);
    
    itemType *found = Find(myList, 4);
    if (found) {
        printf("Found: %d\n", *found);
    } else {
        printf("Not Found\n");
    }
    
    return 0;
}
*/