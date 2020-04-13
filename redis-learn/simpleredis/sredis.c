//
// Created by 盆栽 on 2020/4/12.
//
#include "sredis.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * ========================================
 * String
 * ========================================
 */
String string_new(const char *init) {
    struct sdshdr *sh;
    size_t init_len = strlen(init);

    sh = malloc(sizeof(struct sdshdr) + init_len + 1);
    sh->len = init_len;
    sh->free = 0;

    memcpy(sh->buf, init, init_len);
    sh->buf[init_len] = '\0';

    return (char*)sh->buf;
}



void string_free(String s) {
    if (s == NULL) return;
    free(s - sizeof(struct sdshdr));
}


_Bool string_compare(const String s1, const String s2) {
    size_t l1, l2, min_len;

    _Bool cmp;

    l1 = string_len(s1);
    l2 = string_len(s2);

    min_len =  (l1 < l2) ? l1 : l2;
    cmp = memcmp(s1, s2, min_len);

    if (cmp == 0) return l1 -l2 == 0;
    return !cmp;
}

/*
 * ========================================
 * HashTable Dict
 * ========================================
 */

Dict *dict_new(DictClassMethod *methods);
int dict_expand(Dict *d, unsigned long size);
int dict_add(Dict *d, void *key, void *val);
int dict_replace(Dict *d, void *key, void *val);
int dict_del(Dict *d, const void *key);
int dict_del_without_free(Dict *d, const void *key);

void dict_free(Dict *d);
DictEntry *dict_find(Dict *d, const void *key);

int dict_resize(Dict *d);
DictIterator *dict_get_iterator(Dict *d);
DictEntry *dict_next(DictIterator *iter);
void dict_iterator_free(DictIterator *iter);



int main(int argc, char **argv) {
    String s = string_new("abc");
    String s2 = string_new("abcd");

    string_free(s);

    printf("%s==%s: %d\n", s, s2, string_compare(s, s2));
    return 0;
}
