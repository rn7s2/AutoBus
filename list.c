#include <stdlib.h>
#include "list.h"

struct ListNode* list_node_new(int val, int type)
{
    struct ListNode* tmp = malloc(sizeof(struct ListNode));
    tmp->val = val, tmp->type = type;
    tmp->next = NULL;
    return tmp;
}

int list_first_node_get_val(struct ListNode* head)
{
    return head->next->val;
}

int list_first_node_get_type(struct ListNode* head)
{
    return head->next->type;
}

void list_first_node_remove(struct ListNode* head)
{
    struct ListNode* first_node = head->next;
    head->next = first_node->next;
    free(first_node), first_node = NULL;
}

void list_node_new_append(struct ListNode* head, int val, int type)
{
    struct ListNode* cur = head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = list_node_new(val, type);
}

void list_free(struct ListNode* head)
{
    while (head != NULL) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp), tmp = NULL;
    }
}
