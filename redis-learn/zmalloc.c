//
// Created by 盆栽 on 2020/1/7.
//
#include <stdlib.h>
#include <stdio.h>

void zlibc_free(void *ptr) {
    free(ptr);
}

#include <pthread.h>
#include "zmalloc.h"
#define PREFIX_SIZE (0)

// 我机器上有我知道
//#ifdef HAVE_ATOMIC

#define update_zmalloc_stat_add(__n) __sync_add_and_fetch(&used_memory, (__n))

#define update_zmalloc_stat_sub(__n) __sync_sub_and_fetch(&used_memory, (__n))

#define LONG_SIZE (sizeof(long))

// 对齐代码
// if (_n & (sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \

#define update_zmalloc_stat_alloc(__n) do { \
    size_t _n = (__n); \
    if (_n & LONG_SIZE -1) { \
        _n += LONG_SIZE - (_n % 8 ); \
    } \
    if (zmalloc_thread_safe) { \
        update_zmalloc_stat_sub(_n); \
    } else { \
        used_memory += _n; \
    } \
} while(0)


#define update_zmalloc_stat_free(__n) do { \
    size_t _n = (__n); \
    if (_n & LONG_SIZE -1) { \
        _n += LONG_SIZE - (_n % 8 ); \
    } \
    if (zmalloc_thread_safe) { \
        update_zmalloc_stat_sub(_n); \
    } else { \
        used_memory -= _n; \
    } \
} while(0)


static size_t used_memory = 0;
static int zmalloc_thread_safe = 0;
pthread_mutex_t used_memory_mutex = PTHREAD_MUTEX_INITIALIZER;


static void zmalloc_default_oom(size_t size) {
    fprintf(stderr, "zmalloc: Out of memory trying to allocate %zu bytes\n", size);
    fflush(stderr);
    abort();
}

static void (*zmalloc_oom_handler)(size_t) = zmalloc_default_oom;

void *zmalloc(size_t size) {
    void *ptr = malloc(size+PREFIX_SIZE);

    if (!ptr) {
        zmalloc_oom_handler(size);
    }

    update_zmalloc_stat_alloc(zmalloc_size(ptr));
    return ptr;
}


void *zcalloc(size_t size) {
    void *ptr = calloc(1, size+PREFIX_SIZE);
    if (!ptr) zmalloc_oom_handler(size);
    update_zmalloc_stat_alloc(zmalloc_size(ptr));
    return ptr;
}


void *zrealloc(void *ptr, size_t size) {
    void *realptr;
    size_t oldsize;
    void *newptr;

    if(ptr == NULL) return zmalloc(size);
    oldsize = zmalloc_size(ptr);
    newptr = realloc(ptr, size);
    if (!newptr) zmalloc_oom_handler(size);

    update_zmalloc_stat_free(oldsize);
    update_zmalloc_stat_alloc(zmalloc_size(newptr));
    return newptr;
}


size_t zmalloc_size(void *ptr) {
    void *realptr = (char*)ptr - PREFIX_SIZE;
    size_t size = *((size_t*)realptr);
    if (size&(sizeof(long)-1)) size += sizeof(long)-(size&(sizeof(long)-1));
    return size+PREFIX_SIZE;
}


void zfree(void *ptr) {
    if(ptr == NULL) return;
#ifdef HAVE_MALLOC_SIZE
    update_zmalloc_stat_free(zmalloc_size(ptr));
    free(ptr);
#endif
}
