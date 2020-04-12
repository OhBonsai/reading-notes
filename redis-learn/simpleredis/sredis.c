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
String new_string(const char *init) {
    struct sdshdr *sh;
    size_t init_len = strlen(init);

    sh = malloc(sizeof(struct sdshdr) + init_len + 1);
    sh->len = init_len;
    sh->free = 0;

    memcpy(sh->buf, init, init_len);
    sh->buf[init_len] = '\0';

    return (char*)sh->buf;
}



void free_string(String s) {
    if (s == NULL) return;
    free(s - sizeof(struct sdshdr));
}


_Bool compare_string(const String s1, const String s2) {
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
 * IntSet
 * ========================================
 */
#define INTSET_ENC_INT16 (sizeof(int16_t))
#define INTSET_ENC_INT32 (sizeof(int32_t))
#define INTSET_ENC_INT64 (sizeof(int64_t))

IntSet *NewIntSet(void);
IntSet *IntSetAdd(IntSet *is, int64_t value, uint8_t *success);
IntSet *IntSetRemove(IntSet *is, int64_t value, uint8_t *success);
uint8_t IntSetFind(IntSet *is, int64_t value);
uint8_t IntSetGet(IntSet *is, uint32_t pos, int64_t *value);
uint32_t IntSetLen(IntSet *is);
uint32_t IntSetBlobLen(IntSet *is);


int main(int argc, char **argv) {
    String s = new_string("abc");
    String s2 = new_string("abcd");

    free_string(s);

    printf("%s==%s: %d\n", s, s2, compare_string(s, s2));
    return 0;
}
