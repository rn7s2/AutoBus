#include <stdlib.h>
#include "list.h"

struct ListNode* new_node(int val)
{
    struct ListNode* tmp = malloc(sizeof(struct ListNode));
    tmp->val = val;
    tmp->next = NULL;
    return tmp;
}
