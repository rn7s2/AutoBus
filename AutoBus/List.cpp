#include "List.h"
#include <cstdlib>

struct ListNode* list_node_new(int val, int type)
{
	struct ListNode* tmp = static_cast<struct ListNode*>(malloc(sizeof(struct ListNode)));
	if (tmp == nullptr) {
		return nullptr;
	}
	tmp->val = val, tmp->type = type;
	tmp->next = nullptr;
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
	free(first_node), first_node = nullptr;
}

void list_node_remove(struct ListNode* head, int val, int type)
{
	struct ListNode* cur = head->next;
	struct ListNode* lst = head;
	while (cur) {
		if (cur->val == val && cur->type == type) {
			lst->next = cur->next;
			free(cur), cur = nullptr;
			break;
		}
		lst = cur, cur = cur->next;
	}
}

void list_node_new_append(struct ListNode* head, int val, int type)
{
	struct ListNode* cur = head;
	while (cur->next) {
		cur = cur->next;
	}
	cur->next = list_node_new(val, type);
}

int list_length(struct ListNode* head)
{
	int ans = 0;
	while (head->next) {
		head = head->next;
		++ans;
	}
	return ans;
}

void list_free(struct ListNode* head)
{
	while (head != nullptr) {
		struct ListNode* tmp = head;
		head = head->next;
		free(tmp), tmp = nullptr;
	}
}