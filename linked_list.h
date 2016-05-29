/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

struct linked_list_node {
    void *data;
    struct linked_list_node *next;
};

struct linked_list {
    struct linked_list_node *first;
    struct linked_list_node *last;
    int length;
};

/*
 * Create an empty linked list
 */
struct linked_list
*linked_list_create();

/* 
 * Free the memory used by a linked list. Note that this does not free the
 * memory used by the list's nodes' data.
 */
void
linked_list_destroy(struct linked_list *list);

/*
 * Add a node to the "end" of the list (modifies list->last).
 */
void
linked_list_append(struct linked_list *list, void *data);

/*
 * Add a node to the "beginning" of the list (modifies list->first).
 */
void
linked_list_push(struct linked_list *list, void *data);

/*
 * Remove the first element of the list and return it's data.
 */
void
*linked_list_pop(struct linked_list *list);

#endif /* LINKED_LIST_H */
