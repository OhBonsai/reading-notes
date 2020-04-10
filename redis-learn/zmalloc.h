//
// Created by 盆栽 on 2020/1/7.
//

#ifndef REDIS_LEARN_ZMALLOC_H
#define REDIS_LEARN_ZMALLOC_H

#define __xstr(s) __str(s)
#define __str(s) #s


// 不做特殊处理了， 反正我是mac...

#include <malloc/malloc.h>
#define HAVE_MALLOC_SIZE 1

#ifndef ZMALLOC_LIB
#define ZMALLOC_LIB "libc"
#endif


void *zmalloc(size_t size);
void *zcalloc(size_t size);
void *zrealloc(void *ptr, size_t size);
void zfree(void *ptr);

size_t zmalloc_size(void *ptr);


#endif //REDIS_LEARN_ZMALLOC_H
