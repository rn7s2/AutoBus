/******************************************************************************
 * Filename:    list.h
 * Author:      雷瑞祺
 * Purpose:     Definition of a single-linked list.
 * Last update: 2022-05-15
 ******************************************************************************/

#ifndef _LIST_H
#define _LIST_H

// type: 0 for counterclockwise, 1 for clockwise, 2 for target
struct ListNode {
    int val, type;
    struct ListNode* next;
};

struct ListNode* list_node_new(int val, int type);
int list_first_node_get_val(struct ListNode* head);
int list_first_node_get_type(struct ListNode* head);
void list_first_node_remove(struct ListNode* head);
void list_node_remove(struct ListNode* head, int val, int type);
void list_node_new_append(struct ListNode* head, int val, int type);
int list_length(struct ListNode* head);
void list_free(struct ListNode* head);

#endif
