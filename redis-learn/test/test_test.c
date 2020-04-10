//
// Created by 盆栽 on 2020/3/19.
// 就是本地测试
#include "stdio.h"
#include "stdlib.h"


typedef struct obj {
    unsigned type:4;
    unsigned encode:4;
    unsigned lru:28;

    int ref_count;
    void *ptr;
} Object;



typedef struct node {
    Object* o;
    double score;
    struct node *back;
    struct Level {
        struct node * forward;
        unsigned int span;
    } level[];
} Node;


int main(int argc, char ** argv) {
    Node* o = NULL4;


    printf("%lld", sizeof(*o));

    return 0;
}