#include <stdio.h>
#include "adlist.h"

int main (int argc, char ** argv)
{
    list *t_list = listCreate();
    int a=2, b=3, c=4;


    listAddNodeHead(t_list, &a);
    listAddNodeTail(t_list, &b);
    listInsertNode(t_list, t_list->tail, &c, 0);


    listNode *tmp;
    listIter *iter = listGetIterator(t_list, 0);
    for(;;) {
        tmp = listNext(iter);
        if (tmp == NULL) break;
        printf("%d->", *(int*)(tmp->value));
    }

    return 0;
}