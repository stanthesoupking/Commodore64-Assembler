#pragma once

#include <stdbool.h>

typedef struct linked_list_node_int LinkedListNode;

struct linked_list_node_int {
    void * data;
    LinkedListNode* next;
};

typedef struct linked_list_int {
    LinkedListNode* head;
} LinkedList;

LinkedList* createLinkedList();
void destroyLinkedList(LinkedList* l);

void linkedListInsert(LinkedList* l, void* data);
void linkedListPop(LinkedList* l, unsigned int index);
void linkedListInsertOrdered(LinkedList* l, void* data, bool (*insertHere)(void* a, void* b));
LinkedListNode* linkedListGetHead(LinkedList* l);