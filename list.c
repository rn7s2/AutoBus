#include <stdlib.h>
#include "list.h"

struct ListNode* list_node_new(int val)
{
    struct ListNode* tmp = malloc(sizeof(struct ListNode));
    tmp->val = val;
    tmp->next = NULL;
    return tmp;
}

void list_free(struct ListNode* head)
{
    while (head != NULL) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp), tmp = NULL;
    }
}
