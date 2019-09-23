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

    ListNode *pred = l, *apl = (List)malloc(sizeof(ListNode)), *end = apl;
    apl->link = NULL;
    p = l->link;
    while (p) {
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
    pred->link = apl->link;
    free(apl);

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