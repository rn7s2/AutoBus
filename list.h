#ifndef _LIST_H
#define _LIST_H

struct ListNode {
    int val, is_primary;
    struct ListNode* next;
};

struct ListNode* list_node_new(int);
int list_first_node_get_val(struct ListNode*);
void list_first_node_remove(struct ListNode*);
void list_node_new_append(int);
void list_free(struct ListNode* head);

#endif
