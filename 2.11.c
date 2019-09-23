#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int data;
    ListNode* link;
} ListNode, *List;

int main() {
    List l1, l2;
    l1 = (List)malloc(sizeof(ListNode));
    l2 = (List)malloc(sizeof(ListNode));
    l1->link = l2->link = NULL;

    int num, x;
    scanf("%d %d", &num, &x);
    ListNode *p1 = l1, *p2 = l2;
    for (int i = 0; i < num; i++) {
        int a;
        scanf("%d", &a);
        if (a < x) {
            p1 = p1->link = (ListNode*)malloc(sizeof(ListNode));
            p1->data = a;
        } else {
            p2 = p2->link = (ListNode*)malloc(sizeof(ListNode));
            p2->data = a;
        }
    }
    p1->link = l2->link;
    p2->link = NULL;
    free(l2);

    ListNode* p = l1->link;
    if (p) {
        while (p->link) {
            printf("%d ", p->data);
            p = p->link;
        }
        printf("%d", p->data);
    }

    if (l1) {
        while (l1) {
            p = l1;
            l1 = l1->link;
            free(p);
        }
    }
    return 0;
}