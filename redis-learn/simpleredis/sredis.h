//
// Created by 盆栽 on 2020/4/12.
//

#ifndef REDIS_LEARN_SREDIS_H
#define REDIS_LEARN_SREDIS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


/*
 * ========================================
 * String
 * ========================================
 */
typedef char* String;

struct sdshdr{
    int len;
    int free;
    char buf[];
};

static inline size_t string_len(const String s) {
    struct sdshdr *sh = (void*)(s- sizeof(struct sdshdr));
    return sh->len;
}

static inline size_t string_avail(const String s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

String new_string(const char *init);
void free_string(String s);
_Bool compare_string(const String s1, const String s2);


/*
 * ========================================
 * HashTable
 * ========================================
 */
#define DICT_HT_INITIAL_SIZE 4

typedef struct entry {
    void *key;
    void *val;
    struct entry *next;
} Entry;

typedef struct {
    Entry **table;
    unsigned long size;
    unsigned long size_mask;
    unsigned long used;
} HashTable;

typedef struct {
    unsigned int(*hash_func)(const void *key);
    void* (*copy_key)(const void *key);
    void* (*copy_value)(const void *obj);

    _Bool (*compare_key)(const void *key1, const void *key2);

    void (*free_key)(void *key);
    void (*free_value)(void *obj);
} DictClassMethod;

typedef struct {
    DictClassMethod *methods;
    HashTable *ht;
    int iterator_num;
} Dict;

#define dict_free_val(d, entry) do {\
    if ((d)->type->free_value) \
        (d)->type->free_value((entry)->v.val) \
} while(0)

#define dict_set_val(d, entry, _val_) do { \
    if ((d)->type->copy_value) \
        entry->v.val = (d)->type->copy_value(_val_); \
    else \
        entry->v.val = (_val_); \
} while(0)


#define dict_free_key(d, entry) do \
    if ((d)->type->free_key) \
        (d)->type->free_key((entry)->key)

#define dict_set_key(d, entry, _key_) do { \
    if ((d)->type->copy_key) \
        entry->key = (d)->type->copy_key(_key_); \
    else \
        entry->key = (_key_); \
} while(0)



#endif //REDIS_LEARN_SREDIS_H
