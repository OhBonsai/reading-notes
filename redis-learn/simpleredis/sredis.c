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

Dict *dict_new(DictVoidTable *methods) {
    Dict *d = malloc(sizeof(*d));

    d->ht->table = NULL;
    d->ht->size = 0;
    d->ht->size_mask = 0;
    d->ht->used = 0;

    d->methods = methods;
    d->iterator_num = 0;

    dict_expand(d);
    return d;
}


// block expand....
_Bool dict_expand(Dict *d) {
    unsigned long new_size, new_mask;
    if (d->ht->size == 0) {
        new_size = 4;
    } else {
        new_size = d->ht->size * 2;
    }
    new_mask = new_size - 1;

    DictEntry **table2 = malloc(new_size * sizeof(DictEntry*));
    d->ht->used = 0;


    for (int i=0; i<d->ht->size; i++) {
        DictEntry *de = d->ht->table[i];

        while(de) {
            unsigned int h;

            DictEntry *next_de = de->next;
            h = dict_hash_key(d, de->key) & new_mask;

            de->next = table2[h];
            table2[h] = de;
            d->ht->used ++;

            de = next_de;
        }

    }

    free(d->ht->table);
    d->ht->table = table2;
    d->ht->size = new_size;
    d->ht->size_mask = new_mask;

    return true;
}


_Bool dict_add(Dict *d, void *key, void *val) {
    unsigned int index = dict_hash_key(d, key) & d->ht->size_mask;
    DictEntry *entry = malloc(sizeof(&entry));


    entry->next = d->ht->table[index];
    d->ht->table[index] = entry;
    d->ht->used++;
    dict_set_key(d, entry, key);
    dict_set_val(d, entry, val);

    if (d->ht->used / (d->ht->size * 1.0) > 1.2) dict_expand(d);
    return true;
}


_Bool dict_del(Dict *d, const void *key) {
    DictEntry *de, *pre_de;
    unsigned int h, idx;

    if (d->ht->size == 0) return NULL;

    h = dict_hash_key(d, key);
    idx = h & d->ht->size_mask;
    de = d->ht->table[idx];
    pre_de = NULL;

    while(de) {
        if (dict_compare_key(d, key, de->key)) {
            if (pre_de) {
                pre_de->next = de->next;
            } else {
                d->ht->table[idx] = de->next;
            }

            dict_free_key(d, de);
            dict_free_val(d, de);

            free(de);
            d->ht->used --;
            return true;
        }
        pre_de = de;
        de = de->next;
    }

    return true;
}


DictEntry *dict_find(Dict *d, const void *key) {
    DictEntry *de;
    unsigned int h, idx;

    if (d->ht->size == 0) return NULL;

    h = dict_hash_key(d, key);
    idx = h & d->ht->size_mask;
    de = d->ht->table[idx];

    while(de) {
        if (dict_compare_key(d, key, de->key)) return de;
        de = de->next;
    }

    return NULL;
}


void dict_free(Dict *d) {
    for (int i=0; i<d->ht->size; i++) {
        DictEntry *de, *next_de;
        de = d->ht->table[i];

        while(de) {
            next_de = de->next;
            dict_free_key(d, de);
            dict_free_val(d, de);
            free(de);
            d->ht->used--;
            de = next_de;
        }
    }
    free(d->ht->table);
    d->ht->table = NULL;
    d->ht->size = 0;
    d->ht->size_mask = 0;
    d->ht->used = 0;
    free(d);
}


// 如何解决迭代器期间，entry被释放的问题
//DictIterator *dict_get_iterator(Dict *d) {
//    DictIterator *iter = malloc(sizeof(*iter));
//
//    iter->d = d;
//    iter->index = -1;
//    iter->entry = NULL;
//    iter->next_entry = NULL;
//
//    return iter;
//}
//
//DictEntry *dict_next(DictIterator *iter) {
//    while (1) {
//        if (iter->entry == NULL) {
//            HashTable *ht = iter->d->ht;
//
//
//            if (iter->index == -1 && iter->)
//        }
//    }
//
//    return NULL;
//}
//void dict_iterator_free(DictIterator *iter);



int main(int argc, char **argv) {
//    String s = string_new("abc");
//    String s2 = string_new("abcd");
//
//    string_free(s);
//
//    printf("%s==%s: %d\n", s, s2, string_compare(s, s2));

    printf("%d\n",  3 / (2 * 1.0) < 1);
    return 0;
}
