//
// Created by 盆栽 on 2020/4/19.
//
#include "sredis.h"
#include <stdio.h>

void test_string() {
    printf("===========String============\n");

    String s = string_new("abc");
    String s2 = string_new("abcd");

    printf("%s==%s: %d\n", s, s2, string_compare(s, s2));
}


void test_dict() {
    printf("===========Dict============\n");



    Dict *d = dict_new(&vt);
    Object *k = init_string_obj("abc", 3);
    Object *v = init_string_obj("ab", 2);
    Object *mk = init_string_obj("abc", 3);

    dict_add(d,  k , v);

    printf("%s\n", (String)((Object *)dict_find(d, k)->val)->ptr);

    dict_del(d, k);

    DictEntry *ed = dict_find(d, mk);
    printf("%p\n", ed);

}


void test_object() {

    printf("===========Object============\n");

    Object *s1 = init_string_obj("abc", 3);
    Object *s2 = init_string_obj("ab", 2);
    Object *s3 = init_string_obj("aa", 2);
    Object *s4 = init_string_obj("aa", 2);
    printf("abc==ab %d\n", dict_compare_func(s1, s2));
    printf("ab==aa %d\n", dict_compare_func(s2, s3));
    printf("aa==aa %d\n", dict_compare_func(s4, s3));
}


int main() {
    test_string();
    test_dict();
    test_object();

    return 0;
}
