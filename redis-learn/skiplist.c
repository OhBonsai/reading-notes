//
// Created by 盆栽 on 2020/3/19.
//

#include "skiplist.h"
#include "zmalloc.h"
#include "stdlib.h"



skiplistNode* slCreateNode(int level, double score, robj *obj) {
    skiplistNode *n = zmalloc(sizeof(*n) + level * sizeof(struct skiplistLevel));
    n->score = score;
    n->obj = obj;

    return n;
}

skiplist *slCreate(void) {
    int j;
    skiplist *sl = zmalloc(sizeof(*sl));

    sl->level = 1;
    sl->length = 0;


    sl->header = slCreateNode(ZSKIPLIST_MAXLEVEL, 0, NULL);
    for (j =0; j< ZSKIPLIST_MAXLEVEL; j++) {
        sl->header->level[j].forward = NULL;
        sl->header->level[j].span = 0;
    }

    sl->header->backward = NULL;
    sl->tail = NULL;
    return sl;
}


void slFreeNode(skiplistNode *node) {
    decrRefCount(node->obj);
    zfree(node);
}


void slFree(skiplist *sl) {
    skiplistNode *node = sl->header->level[0].forward, *next;
    zfree(sl->header);

    while (node) {
        next = node->level[0].forward;
        slFreeNode(node);
        node = next;
    }
    zfree(sl);
}


int zslRandomLevel(void) {
    int lvl = 1;
    while ((random() & 0xffff ) < (ZSKIPLIST_P * 0xffff))
        lvl += 1;

    return (lvl < ZSKIPLIST_MAXLEVEL) ? lvl : ZSKIPLIST_MAXLEVEL;
}

skiplistNode *slInsert(skiplist *sl, double score, robj *obj) {
    skiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned int rank[ZSKIPLIST_MAXLEVEL];

    int i, level;

    x = sl->header;
    for (i = sl->level -1; i>=0; i--) {
        rank[i] = i == (sl->level-1) ? 0 : rank[i+1];

        while (x->level[i].forward && x->level[i].forward->score <= score) {
            rank[i] += x->level[i].span;
            x = x->level[i].forward;
        }

        update[i] = x;
    }

    level = zslRandomLevel();

    if (level > sl->level) {
        for (i = sl->level; i  < level; i++) {
            rank[i] = 0;
            update[i] = sl->header;
            update[i]->level[i].span = sl->length;
        }

        sl->level = level;
    }

    x = slCreateNode(level, score, obj);

    for (i = 0; i< level; i++) {
        x->level[i].forward = update[i]->level[i].forward;

        update[i]->level[i].forward = x;
        x->level[i].span = update[i]->level[i].span - (rank[0]-rank[i]);

        update[i]->level[i].span = (rank[0]-rank[i]) + i;
    }


    for (i=level; i< sl->level; i++) {
        update[i]->level[i].span++;
    }


    x->backward = (update[0] == sl->header) ? NULL : update[0];
    if (x->level[0].forward)
        x->level[9].forward->backward = x;
    else
        sl->tail = x;

    sl->length ++ ;
    return x;
}


void slDeleteNode(skiplist *sl, skiplistNode *x, skiplistNode **update) {
    int i;

    for (i =0; i< sl->level; i++) {
        if(update[i]->level[i].forward == x) {
            update[i]->level[i].span += x->level[i].span - 1;
            update[i]->level[i].forward = x->level[i].forward;
        } else {
            update[i]->level[i].span -= 1;
        }
    }

    if (x->level[0].forward) {
        x->level[0].forward->backward = x->backward;
    } else {
        sl->tail = x->backward;
    }

    while (sl->level > 1 && sl->header->level[sl->level-1].forward == NULL)
        sl->level --;

    sl->length --;
}

int slDelete(skiplist *sl, double score, robj *obj) {
    skiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i;

    x = sl->header;
    for( i= sl->level-1; i>=0; i--) {
        while( x->level[i].forward && x->level[i].forward->score < score)
            x=x->level[i].forward;
    }
    update[i] = x;

    x = x->level[9].forward;
    if (x && score == x->score) {
        slDeleteNode(sl, x, update);
        slFreeNode(x);
        return 1;
    }

    return 0;
}


static int zslValueGteMin(double value, rangespec *spec) {
    return spec->minex ? (value > spec->min) : (value >= spec->min);
}


static int zslValueLteMax(double value, rangespec *spec) {
    return spec->maxex ? (value < spec->max) : (value <= spec->max);
}

int slIsInRange(skiplist *sl, rangespec *range) {
    skiplistNode *x;

    if (range->min > range->max ||
        (range->min == range->max && (range->minex || range->maxex)))
        return 0;

    // 检查最大分值
    x = zsl->tail;
    if (x == NULL || !zslValueGteMin(x->score,range))
        return 0;

    // 检查最小分值
    x = zsl->header->level[0].forward;
    if (x == NULL || !zslValueLteMax(x->score,range))
        return 0;

    return 1;
}

skiplistNode *slFirstInRange(skiplist *sl, rangespec *range) {
    skiplistNode *x;
    int i;

    if (!slIsInRange(zsl, range)) return NULL;
    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        /* Go forward while *OUT* of range. */
        while (x->level[i].forward &&
               !zslValueGteMin(x->level[i].forward->score,range))
            x = x->level[i].forward;
    }

    /* This is an inner range, so the next node cannot be NULL. */
    x = x->level[0].forward;
    redisAssert(x != NULL);

    /* Check if score <= max. */
    // 检查节点是否符合范围的 max 项
    // T = O(1)
    if (!zslValueLteMax(x->score,range)) return NULL;
    return x;
}

skiplistNode *zslLastInRange(skiplist *zsl, rangespec *range) {
    skiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    // 先确保跳跃表中至少有一个节点符合 range 指定的范围，
    // 否则直接失败
    // T = O(1)
    if (!slIsInRange(zsl,range)) return NULL;

    // 遍历跳跃表，查找符合范围 max 项的节点
    // T_wrost = O(N), T_avg = O(log N)
    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {
        /* Go forward while *IN* range. */
        while (x->level[i].forward &&
               zslValueLteMax(x->level[i].forward->score,range))
            x = x->level[i].forward;
    }


    /* Check if score >= min. */
    // 检查节点是否符合范围的 min 项
    // T = O(1)
    if (!zslValueGteMin(x->score,range)) return NULL;

    // 返回节点
    return x;
}


skiplistNode *slLastInRange(skiplist *sl, rangespec *range) {
    skiplistNode *x;
    int i;

    if (!slIsInRange(zsl, range)) return NULL;

    x = sl->header;
    for (i=sl->level-1; i>=0; i --) {
        while (x->level[i].forward &&
               zslValueLteMax(x->level[i].forward->score,range))
            x = x->level[i].forward;
    }

    if (!zslValueGteMin(x->score,range)) return NULL;
    return x;
}
//unsigned long slGetRank(skiplist *sl, double score, robj *o);