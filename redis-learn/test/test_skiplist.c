//
// Created by 盆栽 on 2020/3/19.
//
#include "../skiplist.h"
#include "stdio.h"

void test_skiplist_create() {
    skiplist* sl = slCreate();
    printf("%d", sl->level);

}


int main(int argc, char ** argv) {
    test_skiplist_create();

    return 0;
}