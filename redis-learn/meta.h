//
// Created by 盆栽 on 2020/3/19.
//

#ifndef REDIS_LEARN_META_H
#define REDIS_LEARN_META_H

#define REDIS_LRU_BITS 24
#define ZSKIPLIST_MAXLEVEL 32 /* Should be enough for 2^32 elements */
#define ZSKIPLIST_P 0.25


typedef struct redisObject {

    // 类型
    unsigned type:4;

    // 编码
    unsigned encoding:4;

    // 对象最后一次被访问的时间
    unsigned lru:REDIS_LRU_BITS; /* lru time (relative to server.lruclock) */

    // 引用计数
    int refcount;

    // 指向实际值的指针
    void *ptr;

} robj;




typedef struct {
    double min, max;
    int minex, maxex;
} rangespec;



void decrRefCount(robj *o);


#endif //REDIS_LEARN_META_H
