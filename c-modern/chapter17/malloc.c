//
// Created by 盆栽 on 2020/2/1.
//
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>

typedef struct hello {
    int x;

    void (*handler)(void *self, int i);
} Clas;


int hello_handler(Clas *self,int  i) {
    self->x += i;
};


Clas* hello_init(int x) {
    Clas hello = {
            .x = x,
            .handler = hello_handler
    };

    return &hello;
}




int dynamic_handler(int f(int i), int b) {
    return f(b);
}

int add2(int i) {
    return i + 2;
}


int main() {
    size_t size = 2;
    void *p;
    if ( (p=malloc(size)) == NULL) {
        printf("memory out \n");
    }


    printf("%p\n", sbrk(0));


    printf("%d\n", dynamic_handler(add2, 3));


    Clas *ins_ptr = hello_init(2);
    printf("===========");
    printf("%d\n", ins_ptr->x);

    printf("===========");
    ins_ptr->handler(ins_ptr, 3);
    printf("%d\n", ins_ptr->x);


    return 0;
}


