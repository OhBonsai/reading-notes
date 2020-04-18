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

String string_new(const char *init);
void string_free(String s);
_Bool string_compare(const String s1, const String s2);


/*
 * ========================================
 * HashTable
 * ========================================
 */
typedef struct entry {
    void *key;
    void *val;
    struct entry *next;
} DictEntry;

typedef struct {
    DictEntry **table;
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
} DictVoidTable;

typedef struct {
    DictVoidTable *methods;
    HashTable *ht;
} Dict;

typedef struct {
    Dict *d;
    int index;
    DictEntry *entry, *next_entry;
} DictIterator;

#define dict_free_val(d, entry) do { \
    if ((d)->methods->free_value) \
        (d)->methods->free_value((entry)->val); \
} while(0)

#define dict_set_val(d, entry, _val_) do { \
    if ((d)->methods->copy_value) \
        entry->val = (d)->methods->copy_value(_val_); \
    else \
        entry->val = (_val_); \
} while(0)


#define dict_free_key(d, entry) do {\
    if ((d)->methods->free_key) \
        (d)->methods->free_key((entry)->key); \
} while(0)

#define dict_set_key(d, entry, _key_) do { \
    if ((d)->methods->copy_key) \
        entry->key = (d)->methods->copy_key(_key_); \
    else \
        entry->key = (_key_); \
} while(0)

#define dict_compare_key(d, key1, key2) \
    (((d)->methods->compare_key) ? \
        (d)->methods->compare_key(key1, key2) :\
        (key1) == (key2))

#define dict_hash_key(d, key) (d)->methods->hash_func(key)
#define entry_key(entry) ((entry)->key)
#define entry_val(entry) ((entry)->val)
#define dict_slots(d) ((d)->ht.size)
#define dict_size(d) ((d)->ht.used)

Dict *dict_new(DictVoidTable *methods);
_Bool dict_expand(Dict *d);
_Bool dict_add(Dict *d, void *key, void *val);
_Bool dict_del(Dict *d, const void *key);

void dict_free(Dict *d);
DictEntry *dict_find(Dict *d, const void *key);

DictIterator *dict_get_iterator(Dict *d);
DictEntry *dict_next(DictIterator *iter);
void dict_iterator_free(DictIterator *iter);








#endif //REDIS_LEARN_SREDIS_H
