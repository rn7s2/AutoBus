#ifndef _LIST_H
#define _LIST_H

struct ListNode {
    int val;
    struct ListNode* next;
};

struct ListNode* new_node(int);
int get_first_node_val(struct ListNode*);
void remove_first_node(struct ListNode*);
void append_new_node(int);
void free_list(struct ListNode* head);

#endif
