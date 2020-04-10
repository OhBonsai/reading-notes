//
// Created by 盆栽 on 2020/1/7.
//
#include "sds.h"
#include "zmalloc.h"
#include "string.h"


sds sdsnewlen(const void *init, size_t initlen){
    struct sdshdr *sh;


    if (init) {
        sh = zmalloc(sizeof(struct sdshdr) + initlen + 1);
    } else {
        sh = zcalloc(sizeof(struct sdshdr) + initlen + 1);
    }


    if (sh == NULL) return NULL;


    sh->len = initlen;
    sh->free = 0;

    if (initlen && init)
        memcpy(sh->buf, init, initlen);
    sh->buf[initlen] = '\0';

    return (char*)sh->buf;
}


sds sdsnew(const char *init) {
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}


sds sdsempty(void) {
    return sdsnewlen("", 0);
}


sds sdsdup(const sds s) {
    return sdsnewlen(s, sdslen(s));
}

void sdsfree(sds s) {
    if (s == NULL) return;
    zfree(s - sizeof(struct sdshdr));
}


sds sdscatlen(sds s, const void *t, size_t len) {
    struct sdshdr *sh;

    size_t curlen = sdslen(s);
    s = sdsMakeRoomFor(s, len);

    if (s == NULL) return NULL;

    sh = (void*) (s-(sizeof(struct sdshdr)));
    memcpy(s+curlen, t, len);

    sh->len = curlen + len;
    sh->free = sh->free - len;

    s[curlen+len] = '\0';
    return s;
}


sds sdscat(sds s, const char *t) {
    return sdscatlen(s, t, strlen(t));
}

sds sdscatsds(sds s, const sds t) {
    return sdscatlen(s, t, sdslen(t));
}

sds sdscpylen(sds s, const char *t, size_t len) {
    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));

    size_t totlen = sh->free+sh->len;

    if (totlen < len) {
        s = sdsMakeRoomFor(s, len-sh->len);
        if (s == NULL) return NULL;
        sh = (void*) (s - (sizeof(struct sdshdr)));
        totlen = sh->free+sh->len;
    }

    memcpy(s, t, len);
    s[len] = '\0';

    sh->len = len;
    sh->free = totlen -len;
    return s;
}

sds sdscpy(sds s, const char *t) {
    return sdscpylen(s, t, strlen(t));
}

void sdsclear(sds s) {
    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
    sh->free += sh->len;
    sh->len = 0;

    sh->buf[0] = '\0';
}


void sdsrange(sds s, int start, int end) {
    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
    size_t newlen, len = sdslen(s);

    if (len == 0) return ;
    if (start < 0) {
        start = len + start;
        if(start < 0) start = 0;
    }

    if (end < 0 ) {
        end = len + end;
        if (end < 0) end = 0;
    }

    newlen = (start > end ) ? 0 : (end-start) + 1;
    if (newlen != 0 ) {
        if (start >= (signed)len) {
            newlen = 0;
        } else if (end >= (signed)len) {
            end = len -1;
            newlen = (start > end) ? 0 : (end-start) + 1;
        }
    } else {
        start = 0;
    }


    if (start && newlen) memmove(sh->buf, sh->buf+start, newlen);
    sh->buf[newlen] = 0;
    sh->free = sh->free+(sh->len-newlen);
    sh->len = newlen;
}


sds sdstrim(sds s, const char *cset) {
    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
    char *start, *end, *sp, *ep;
    size_t  len;

    sp = start = s;
    ep = end = s+sdslen(s)-1;

    while(sp <=end && strchr(cset, *sp)) sp ++;
    while(ep > start && strchr(cset, *ep)) ep --;

    len = (sp > ep) ? 0 : ((ep-sp)+1);

    if (sh->buf != sp) memmove(sh->buf, sp, len);
    sh->buf[len] = '\0';

    sh->free = sh->free + (sh->len-len);
    sh->len = len;
    return s;
}


int sdscmp(const sds s1, const sds s2) {
    size_t l1, l2, minlen;
    int cmp;

    l1 = sdslen(s1);
    l2 = sdslen(s2);

    minlen = (l1 < l2) ? l1 : l2;
    cmp = memcmp(s1, s2, minlen);

    if (cmp ==0) return l1 -l2;
    return cmp;
}

sds sdsMakeRoomFor(sds s, size_t addlen) {
    struct sdshdr *sh, *newsh;
    size_t free =  sdsavail(s);
    size_t len, newlen;


    if (free >= addlen) return s;
    len = sdslen(s);
    sh = (void*) (s-(sizeof(struct sdshdr)));
    newlen = (len+addlen);


    if (newlen < SDS_MAX_PREALLOC)
        newlen *= 2;
    else
        newlen += SDS_MAX_PREALLOC;

    newsh = zrealloc(sh, sizeof(struct sdshdr) + newlen + 1);

    if (newsh == NULL) return NULL;

    newsh->free = newlen - len;
    return newsh->buf;
}