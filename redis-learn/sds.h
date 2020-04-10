//
// Created by 盆栽 on 2020/1/7.
//

#ifndef REDIS_LEARN_SDS_H
#define REDIS_LEARN_SDS_H

#define SDS_MAX_PREALLOC (1024 * 1024)

#include <sys/types.h>
#include <stdarg.h>


typedef char* sds;

struct sdshdr {
    int len;
    int free;
    char buf[];
};


static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}


static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}


size_t sdslen(const sds s);
size_t sdsavail(const sds s);



sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);
sds sdsempty(void);
sds sdsdup(const sds s);
void sdsfree(sds s);
void sdsclear(sds s);

sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, const char *t);

sds sdscatsds(sds s, const sds t);
sds sdscpy(sds s, const char *t);

void sdsrange(sds s, int start, int end);
sds sdstrim(sds, const char *cset);
int sdscmp(const sds s1, const sds s2);



// low level function
sds sdsMakeRoomFor(sds s, size_t addlen);


#endif //REDIS_LEARN_SDS_H


