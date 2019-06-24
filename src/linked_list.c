#include "linked_list.h"

#include <stdlib.h>

LinkedList* createLinkedList()
{
    LinkedList* l = (LinkedList*) malloc(sizeof(struct linked_list_int));

    l->head = NULL;

    return l;
}

void destroyLinkedList(LinkedList* l)
{
    // TODO: Free linked list elements
    // ...

    free(l);
    l = NULL;
}

void linkedListInsert(LinkedList* l, void* data)
{
    LinkedListNode* n;

    // Create node
    n = (LinkedListNode*) malloc(sizeof(struct linked_list_node_int));
    n->data = data;

    // Insert into list
    n->next = l->head;
    l->head = n;
}

void linkedListPop(LinkedList* l, unsigned int index)
{
    unsigned int i;
    LinkedListNode* n;
    LinkedListNode* p;
    
    // If linked list is already empty don't do anything
    if(l->head == NULL)
    {
        return;
    }
    
    if(index == 0)
    {
        n = l->head;
        l->head = n->next;
        // Free node
        free(n);
    }
    else
    {   
        p = l->head;
        n = p->next;
        i = 1;
        while(n != NULL)
        {
            if(i == index)
            {
                p->next = n->next;
                free(n);
                break;
            }

            // Move to next item
            p = n;
            n = n->next;
            i++;
        }
    }
}

void linkedListInsertOrdered(LinkedList* l, void* data,
    bool (*insertHere)(void* a, void* b))
{
    LinkedListNode* n;
    LinkedListNode* node;
    bool validPlace;

    // Create node
    n = (LinkedListNode*) malloc(sizeof(struct linked_list_node_int));
    n->data = data;

    // If linked list is empty...
    if(l->head == NULL)
    {
        // Do regular insert at front of list.
        linkedListInsert(l, data);
        return;
    }

    // Check if the insert position is the head of the list
    if(insertHere(data, l->head->data))
    {
        n->next = l->head;
        l->head = n;
        return;
    }

    // Find place to insert data
    validPlace = false;
    node = l->head;
    while(node != NULL) {
        if(node->next == NULL || insertHere(data, node->next->data))
        {
            n->next = node->next;
            node->next = n;
            return;
        }
        node = node->next;
    }
    
}

LinkedListNode* linkedListGetHead(LinkedList* l)
{
    return l->head;
}