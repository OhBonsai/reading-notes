//
// Created by 盆栽 on 2020/3/4.
//
#include "../dict.h"
#include <assert.h>
#include <stdio.h>

void test_timeInMilliseconds() {
    long long x = timeInMilliseconds();
    printf("%lld\n", x);
}


void test_addKeyInDict() {
    dict *d = dictCreate(NULL, NULL);
    char *key = "abc";
    int value = 2;

    dictAdd(d, key, &value);

//    printf("%p", dictGetRandomKey(d));
}

int main(int argc, char **argv) {
    test_timeInMilliseconds();
    printf("%s\n", "--------------------------------");

    test_addKeyInDict();
    printf("%s\n", "--------------------------------");

    return 0;
}