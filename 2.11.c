#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int data;
    ListNode* link;
} ListNode, *List;

int main() {
    List l;
    l = (List)malloc(sizeof(ListNode));
    l->link = NULL;

    int num, x;
    scanf("%d %d", &num, &x);
    ListNode* p = l;
    for (int i = 0; i < num; i++) {
        p = p->link = (ListNode*)malloc(sizeof(ListNode));
        scanf("%d", &p->data);
    }
    p->link = NULL;

    ListNode *oend = p, *end = p, *pred = l;
    p = l->link;
    while (p != oend) {
        if (p->data >= x) {
            end = end->link = p;
            pred->link = p->link;
            p->link = NULL;
            p = pred->link;
        } else {
            pred = p;
            p = p->link;
        }
    }

    p = l->link;
    if (p) {
        while (p->link) {
            printf("%d ", p->data);
            p = p->link;
        }
        printf("%d", p->data);
    }

    if (l) {
        while (l) {
            p = l;
            l = l->link;
            free(p);
        }
    }
    return 0;
}