//
// Created by 盆栽 on 2020/1/31.
//
#include <stdio.h>
#include <string.h>

#define print_ptr(x) printf(#x ": %p\n", x)

struct {
    int number;
} n1 = {2},
  n2 = {3};


typedef struct part {
    int number;
    char name[15];
} Part;


typedef struct union_test {
    int number;
    enum {INT_KIND, DOUBLE_KIND} kind;
    union {
        int i;
        double d;
    };
} UnionTest;


typedef enum suit {
    A = 2,
    B,
    C
} Suit;

void print_part(Part p) {
    printf("%p\n", &p);
    printf("PartNumber is %d\n", p.number);
    printf("PartName is %s\n", p.name);
}

Part create_part(int number,const char *x) {
    Part p;

    p.number = number;
    strcpy(p.name, x);

    printf("%p\n", &p);
    return p;
}

Part do_nothing(Part p) {
    print_ptr(&p);
    return p;
}

int main() {
    Part p = create_part(2, "abc");
    print_ptr(&p);

    print_part(p);

    Part p2 = do_nothing(p);
    print_ptr(&p2);


    UnionTest ut = {
            .number = 2,
            .d = 2.2,
    };
    print_ptr(&ut);
    printf("%d\n", sizeof(UnionTest));

    printf("%s\n", __func__);


    Suit s1;
    s1 = B;
    printf("%d\n", s1);
    printf("%d\n", sizeof(Suit));


    return 0;
}

