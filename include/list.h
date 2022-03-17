#ifndef LIST_H
#define LIST_H

#include "errno.h"

/**
 * Singly linked list implementation.
 * 
 * This list do caching the last list node
 * for O(1) performance when a new node is added.
 */

#define LIST_HEAD(name) \
        struct list_node name = { &(name), &(name) }

struct list_node {
    struct list_node *next, *last;
};

static inline uint list_add(struct list_node *head,
                            struct list_node *new_node)
{
    if (!head) {
        fprintf(stderr, "%s: head pointer not initialized!\n\n", __func__);
        return -ENULLPTR;
    }

    head->last->next = new_node;
    head->last = new_node;
    return 0;
}

#define container_of(ptr, type, member) ({                  \
        void *_ptr = (void *)(ptr);                         \
        ((type *)(_ptr - (size_t)&((type *)0)->member)); })

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos; pos = pos->next)

#endif
