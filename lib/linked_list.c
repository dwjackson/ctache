/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#include "linked_list.h"

struct linked_list
*linked_list_create()
{
    struct linked_list *list = malloc(sizeof(*list));
    if (list != NULL) {
        list->first = NULL;
        list->last = NULL;
        list->length = 0;
    }
    return list;
}

void
linked_list_destroy(struct linked_list *list)
{
    struct linked_list_node *curr = list->first;
    struct linked_list_node *prev = NULL;

    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(list);
}

static struct linked_list_node
*create_linked_list_node(void *data)
{
    struct linked_list_node *node = malloc(sizeof(*node));
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

void
linked_list_append(struct linked_list *list, void *data)
{
    struct linked_list_node *node;
    node = create_linked_list_node(data);
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
        if (list->length == 0) {
            list->first = node;
            list->last = node;
        } else if (list->length == 1) {
            list->first->next = node;
            list->last = node;
        } else {
            list->last->next = node;
            list->last = node;
        }
        (list->length)++;
    }
}

void
linked_list_push(struct linked_list *list, void *data)
{
    struct linked_list_node *node = create_linked_list_node(data);
    if (node != NULL) {
        if (list->length == 0) {
            list->first = node;
            list->last = node;
        } else if (list->length == 1) {
            node->next = list->last;
            list->first = node;
        } else {
            node->next = list->first;
            list->first = node;
        }
        (list->length)++;
    }
}

/*
 * Remove the first element of the list and return it's data.
 */
void
*linked_list_pop(struct linked_list *list)
{
    void *data;
    struct linked_list_node *node;
    if (list->length == 0) {
        data = NULL;
    } else if (list->length == 1) {
        node = list->first;
        list->first = NULL;
        list->last = NULL;
        list->length = 0;
        data = node->data;
        free(node);
    } else {
        node = list->first;
        list->first = list->first->next;
        data = node->data;
        free(node);
        (list->length)--;
    }
    return data;
}

void
*linked_list_peek(struct linked_list *list)
{
    struct linked_list_node *node;
    void *data = NULL;
    if (list->length > 0) {
        node = list->first;
        data = node->data;
    }
    return data;
}
