//
// Created by 盆栽 on 2020/4/12.
//
#include "sredis.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*
 * ========================================
 * Tool
 * ========================================
 */
long long ms(void) {
    struct timeval tv;
    long long ust;

    gettimeofday(&tv, NULL);
    ust = ((long long) tv.tv_sec) * 1000000;
    ust += tv.tv_usec;
    return ust / 1000;
}


/*
 * ========================================
 * Data Structure
 * ========================================
 */


/*
 * ================String================
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
 * ================Dict================
 */

unsigned int dict_hash_func(const void *key) {
    uint32_t seed = 5381;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;
    const unsigned char *data = (const unsigned char *) key;
    int len = strlen(data);
    uint32_t h = seed ^len;


    while (len >= 4) {
        uint32_t k = *(uint32_t *) data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int) h;
}


_Bool dict_compare_func(const void *key1, const void *key2) {
    if (key1 == key2) return true;

    return string_compare(
            (String)((Object *)key1)->ptr,
            (String)((Object *)key2)->ptr
    );
}

DictVoidTable vt = {
        dict_hash_func,
        NULL,
        NULL,
        dict_compare_func,
        NULL,
        NULL
};


Dict *dict_new(DictVoidTable *methods) {
    Dict *d = malloc(sizeof(*d));
    HashTable *ht = malloc(sizeof(*ht));

    d->ht = ht;
    ht->table = NULL;
    ht->size = 0;
    ht->size_mask = 0;
    ht->used = 0;

    d->methods = methods;

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

    DictEntry **table2 = calloc(new_size, sizeof(DictEntry*));
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

    if (d->ht->size == 0 || d->ht->used == 0) return NULL;

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

    if (d->ht->size == 0 || d->ht->used == 0) return NULL;

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
// 没有StepRehash不用考虑这个问题
DictIterator *dict_get_iterator(Dict *d) {
    DictIterator *iter = malloc(sizeof(*iter));

    iter->d = d;
    iter->index = -1;
    iter->entry = NULL;
    iter->next_entry = NULL;

    return iter;
}

DictEntry *dict_next(DictIterator *iter) {
    while (1) {
        if (iter->entry == NULL) {
            HashTable *ht = iter->d->ht;
            iter->index++;

            if (iter->index >= (signed) ht->size) {
                break;
            }

            iter->entry = ht->table[iter->index];
        } else {
            iter->entry = iter->next_entry;
        }

        if (iter->entry) {
            iter->next_entry = iter->entry->next;
            return iter->entry;
        }
    }

    return NULL;
}

void dict_iterator_free(DictIterator *iter) {
    free(iter);
}


/*
 * ========================================
 * Object
 * ========================================
 */
void inc_ref(Object *o) {
    o->refcount++;
}


void dec_ref(Object *o) {
    if (o->refcount == 1) {
        switch(o->type) {
            case STR_TYPE: break;
            case HASH_TYPE: dict_free((Dict*)o->ptr); break;
        }
        free(o);
    } else {
        o->refcount--;
    }
}



// String
Object *init_string_obj(char *ptr, size_t len) {
    Object *o = malloc(sizeof(Object) + sizeof(String) + len + 1);
    struct sdshdr *sh = (void*)(o+1);

    o->type = STR_TYPE;
    o->lru = ms();
    o->ptr = sh + 1;
    o->refcount = 1;

    sh->len = len;
    sh->free = 0;

    if (ptr) {
        memcpy(sh->buf, ptr, len);
        sh->buf[len] = '\0';
    } else {
        memset(sh->buf, 0, len+1);
    }
    return o;
}


// hashtable
Object *init_hash_obj(void) {
    Dict *d = dict_new(&vt);
    Object *o = malloc(sizeof(*o));

    o->type = HASH_TYPE;
    o->ptr = d;
    o->refcount = 1;
    o->lru = ms();

    return o;
}

/*
 * ========================================
 * DB
 * ========================================
 */
//struct Server server;

void init_server() {


}

