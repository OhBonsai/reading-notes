//
// Created by 盆栽 on 2020/3/19.
//


#ifndef REDIS_LEARN_SKIPLIST_H
#define REDIS_LEARN_SKIPLIST_H

#include "meta.h"

typedef struct skiplistNode {
    robj *obj;
    
    double score;
    
    struct  skiplistNode *backward;
    
    struct skiplistLevel {
        struct  skiplistNode * forward;
        
        unsigned int span;
    } level[];
} skiplistNode;


typedef struct skiplist {
    skiplistNode *header, *tail;
    
    unsigned long length;
    int level;
} skiplist;


skiplist *slCreate(void);
void slFree(skiplist *sl);
skiplistNode *slInsert(skiplist *sl, double score, robj *obj);
int slDelete(skiplist *sl, double score, robj *obj);
skiplistNode *slFirstInRange(skiplist *sl, rangespec *range);
skiplistNode *slLastInRange(skiplist *sl, rangespec *range);
unsigned long slGetRank(skiplist *sl, double score, robj *o);




#endif //REDIS_LEARN_SKIPLIST_H
