//
// Created by 盆栽 on 2020/1/22.
//

#include "dict.h"
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <sys/time.h>
#include <limits.h>

#include "zmalloc.h"


static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;


static int _dictExpandIfNeeded(dict *ht);
static unsigned long _dictNextPower(unsigned long size);
static int _dictKeyIndex(dict *ht, const void *key);
static int _dictInit(dict *ht, dictType *type, void *privDataPrt);


unsigned int dictIntHashFunction(unsigned int key)
{
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}

unsigned int dictIdentityHashFunction(unsigned int key)
{
    return key;
}

static uint32_t dict_hash_function_seed = 5381;
unsigned int dictGenHashFunction(const void *key, int len) {
    /* 'm' and 'r' are mixing constants generated offline.
     They're not really 'magic', they just happen to work well.  */
    uint32_t seed = dict_hash_function_seed;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */
    uint32_t h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */
    const unsigned char *data = (const unsigned char *)key;

    while(len >= 4) {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */
    switch(len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0]; h *= m;
    };

    /* Do a few final mixes of the hash to ensure the last few
     * bytes are well-incorporated. */
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int)h;
}

/* And a case insensitive hash function (based on djb hash) */
unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len) {
    unsigned int hash = (unsigned int)dict_hash_function_seed;

    while (len--)
        hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
    return hash;
}


static void _dictReset(dictht *ht) {
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}


dict *dictCreate(dictType *type, void *privDataPtr) {
    dict *d = zmalloc(sizeof(*d));
    _dictInit(d, type, privDataPtr);
    return d;
}


int _dictInit(dict *d, dictType *type, void *privDataPtr) {
    _dictReset(&(d->ht[0]));
    _dictReset(&(d->ht[1]));

    d->type = type;
    d->privdata = privDataPtr;
    d->rehashidx = -1;
    d->iterators = 0;
    return DICT_OK;
}


int dictResize(dict *d) {
    int minimal;

    if (!dict_can_resize || dictIsRehashing(d)) return DICT_ERR;

    minimal = d->ht[0].used;
    if (minimal < DICT_HT_INITIAL_SIZE)
        minimal = DICT_HT_INITIAL_SIZE;


    return dictExpand(d, minimal);
}

int dictExpand(dict *d, unsigned long size) {
    dictht n;

    unsigned long realsize = _dictNextPower(size);
    if (dictIsRehashing(d) || d->ht[0].used > size)
        return DICT_ERR;


    n.size = realsize;
    n.size = realsize - 1;
    n.table = zcalloc(realsize * sizeof(dictEntry*));
    n.used = 0;

    if (d->ht[0].table == NULL) {
        d->ht[0] = n;
        return DICT_OK;
    }


    d->ht[1] = n;
    d->rehashidx = 0;
    return DICT_OK;
}


int dictRehash(dict *d, int n) {
    if (!dictIsRehashing(d)) return 0;

    while (n--) {
        dictEntry *de, *nextde;


        if (d->ht[0].used == 0) {
            zfree(d->ht[0].table);
            d->ht[0] = d->ht[1];
            _dictReset(&d->ht[1]);
            d->rehashidx = -1;
            return 0;
        }

        assert(d->ht[0].size > (unsigned)d->rehashidx);

        while(d->ht[0].table[d->rehashidx] == NULL )  d->rehashidx++;

        de = d->ht[0].table[d->rehashidx];
        while(de) {
            unsigned int h;
            nextde = de->next;
            h = dictHashKey(d, de->key) & d->ht[1].sizemask;

            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;

            d->ht[0].used --;
            d->ht[1].used ++;

            de = nextde;
        }

        d->ht[0].table[d->rehashidx] = NULL;
        d->rehashidx++;
    }

    return 1;
}


long long timeInMilliseconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ( ((long long)tv.tv_sec) * 1000 ) + (tv.tv_usec/1000);

}


int dictRehashMilliseconds(dict *d, int ms) {
    long long start = timeInMilliseconds();
    int rehashes = 0;

    while (dictRehash(d, 100)) {
        rehashes += 100;
        if (timeInMilliseconds()-start > ms) break;
    }
    return rehashes;
}

static void _dictRehashStep(dict *d) {
    if (d->iterators == 0) dictRehash(d, 1);
}


int dictAdd(dict *d, void *key, void *val) {
    dictEntry *entry = dictAddRaw(d, key);
    if (!entry) return DICT_ERR;
    dictSetVal(d, entry, val);
    return DICT_OK;
}


dictEntry *dictAddRaw(
    dict *d,
    void *key
) {
    int index;
    dictEntry *entry;
    dictht *ht;

    if (dictIsRehashing(d)) _dictRehashStep(d);

    if ((index = _dictKeyIndex(d, key)) == -1) return NULL;

    ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];
    entry = zmalloc(sizeof(&entry));

    entry->next = ht->table[index];  // TODO: 为什么把节点next指向list
    ht->table[index] = entry;
    ht->used++;

    dictSetKey(d, entry, key);
    return entry;
}


int dictReplace(dict *d, void *key, void *val)
{
    dictEntry *entry, auxentry;

    if (dictAdd(d, key, val) == DICT_OK) return 1;

    entry = dictFind(d, key);
    auxentry = *entry;
    dictSetVal(d, entry, val);
    dictFreeVal(d, &auxentry);
    return 0;
}


dictEntry *dictReplaceRaw(dict *d, void *key) {
    dictEntry *entry = dictFind(d, key);
    return entry ? entry : dictAddRaw(d, key);
}


static int dictGenericDelete(dict *d, const void *key, int nofree) {
    unsigned  int h, idx;
    dictEntry *he, *prevHe;
    int table;

    if (d->ht[0].size == 0) return DICT_ERR;

    if (dictIsRehashing(d)) _dictRehashStep(d);

    h = dictHashKey(d, key);

    for (table=0; table <=1; table++) {
        idx = h & d->ht[table].sizemask;

        he = d->ht[table].table[idx];
        prevHe = NULL;

        while(he) {
            if (dictCompareKeys(d, key, he->key)) {
                if(prevHe)
                    prevHe->next = he->next;
                else
                    d->ht[table].table[idx] = he->next;

                if (!nofree) {
                    dictFreeKey(d, he);
                    dictFreeVal(d, he);
                }

                zfree(he);
                d->ht[table].used --;
                return DICT_OK;
            }

            prevHe = he;
            he = he->next;
        }

        if (!dictIsRehashing(d)) break;
    }

    return DICT_ERR;
}

int dictDelete(dict *ht, const void *key) {
    return dictGenericDelete(ht, key, 0);
}

int dictDeleteNoFree(dict *ht, const void *key) {
    return dictGenericDelete(ht, key, 1);
}

int _dictClear(dict *d, dictht *ht, void(callback)(void *)) {
    unsigned long i;

    for (i =0; i< ht->size && ht->used > 0; i++) {
        dictEntry *he , *nextHe;
        if (callback && (i & 65535) == 0) callback(d->privdata);

        if ((he = ht->table[i]) == NULL) continue;

        while(he) {
            nextHe = he->next;
            dictFreeKey(d, he);
            dictFreeVal(d, he);
            zfree(he);
            ht->used--;
            he= nextHe;
        }
    }

    zfree(ht->table);
    _dictReset(ht);
    return DICT_OK;
}


void dictRelease(dict *d) {
    _dictClear(d , &d->ht[0], NULL);
    _dictClear(d , &d->ht[1], NULL);
    zfree(d);
}


dictEntry *dictFind(dict *d, const void *key) {
    dictEntry *he;
    unsigned int h, idx, table;

    if (d->ht[0].size == 0) return NULL;

    if (dictIsRehashing(d)) _dictRehashStep(d);

    h = dictHashKey(d, key);
    for (table =0; table <=1; table ++) {
        idx = h & d->ht[table].sizemask;

        he = d->ht[table].table[idx];

        while (he) {
            if (dictCompareKeys(d, key, he->key)) return he;
            he = he->next;
        }

        if (!dictIsRehashing(d)) return NULL;
    }

    return NULL;
}

void *dictFetchValue(dict *d, const void *key) {
    dictEntry *he;
    he = dictFind(d, key);
    return he ? dictGetVal(he) : NULL;
}

long long dictFingerprint(dict *d) {
    long long integers[6], hash = 0;
    int j;

    integers[0] = (long) d->ht[0].table;
    integers[1] = d->ht[0].size;
    integers[2] = d->ht[0].used;
    integers[3] = (long) d->ht[1].table;
    integers[4] = d->ht[1].size;
    integers[5] = d->ht[1].used;

    /* We hash N integers by summing every successive integer with the integer
     * hashing of the previous sum. Basically:
     *
     * Result = hash(hash(hash(int1)+int2)+int3) ...
     *
     * This way the same set of integers in a different order will (likely) hash
     * to a different number. */
    for (j = 0; j < 6; j++) {
        hash += integers[j];
        /* For the hashing step we use Tomas Wang's 64 bit integer hash. */
        hash = (~hash) + (hash << 21); // hash = (hash << 21) - hash - 1;
        hash = hash ^ (hash >> 24);
        hash = (hash + (hash << 3)) + (hash << 8); // hash * 265
        hash = hash ^ (hash >> 14);
        hash = (hash + (hash << 2)) + (hash << 4); // hash * 21
        hash = hash ^ (hash >> 28);
        hash = hash + (hash << 31);
    }
    return hash;
}



dictIterator *dictGetIterator(dict *d) {
    dictIterator *iter = zmalloc(sizeof(*iter));

    iter->d = d;
    iter->table = 0;
    iter->index = -1;
    iter->safe = 0;
    iter->entry = NULL;
    iter->nextEntry = NULL;

    return iter;
}


dictIterator *dictGetSafeIterator(dict *d) {
    dictIterator *i = dictGetIterator(d);
    i->safe = 1;
    return i;
}


dictEntry *dictNext(dictIterator *iter) {
    while (1) {
        if (iter->entry == NULL) {
            dictht *ht = &iter->d->ht[iter->table];

            if (iter->index == -1 && iter->table == 0) {
                if (iter->safe)
                    iter->d->iterators++;
                else
                    iter->fingerprint = dictFingerprint(iter->d);
            }

            iter->index++;

            if (iter->index >= (signed) ht->size) {
                if(dictIsRehashing(iter->d) && iter->table == 0) {
                    iter->table++;
                    iter->index = 0;
                    ht = &iter->d->ht[1];
                } else {
                    break;
                }
            }

            iter->entry = ht->table[iter->index];
        } else {
            iter->entry = iter->nextEntry;
        }

        if (iter->entry) {
            iter->nextEntry = iter->entry->next;
            return iter->entry;
        }
    }

    return NULL;

}

void dictReleaseIterator(dictIterator *iter) {
    if (!(iter->index == -1 && iter->table == 0)) {
        if (iter->safe)
            iter->d->iterators --;
        else
            assert(iter->fingerprint == dictFingerprint(iter->d));
    }
    zfree(iter);
}

dictEntry *dictGetRandomKey(dict *d) {
    dictEntry *he, *orighe;
    unsigned int h;
    int listlen, listele;

    if (dictSize(d) == 0) return NULL;

    if (dictIsRehashing(d)) _dictRehashStep(d);

    if (dictIsRehashing(d)) {
        do {
            h = random() % (d->ht[0].size+d->ht[1].size);
            he = (h >= d->ht[0].size ? d->ht[1].table[h - d->ht[0].size] : d->ht[0].table[h]);
        } while(he == NULL);
    } else {
        do {
            h = random() & d->ht[0].sizemask;
            he = d->ht[0].table[h];
        } while(he == NULL);
    }

    listlen =0;
    orighe = he;

    while (he) {
        he = he->next;
        listlen++;
    }

    listele = random() % listlen;
    he = orighe;

    while(listele--) he = he->next;
    return he;
}


static unsigned long rev(unsigned long v) {
    unsigned long s = 8 * sizeof(v); // bit size; must be power of 2
    unsigned long mask = ~0;
    while ((s >>= 1) > 0) {
        mask ^= (mask << s);
        v = ((v >> s) & mask) | ((v << s) & ~mask);
    }
    return v;
}


unsigned long dictScan(dict *d,
                       unsigned long v,
                       dictScanFunction *fn,
                       void *privdata) {
    dictht *t0, *t1;
    const dictEntry *de;
    unsigned long m0, m1;

    // 跳过空字典
    if (dictSize(d) == 0) return 0;

    // 迭代只有一个哈希表的字典
    if (!dictIsRehashing(d)) {

        // 指向哈希表
        t0 = &(d->ht[0]);

        // 记录 mask
        m0 = t0->sizemask;

        /* Emit entries at cursor */
        // 指向哈希桶
        de = t0->table[v & m0];
        // 遍历桶中的所有节点
        while (de) {
            fn(privdata, de);
            de = de->next;
        }

        // 迭代有两个哈希表的字典
    } else {

        // 指向两个哈希表
        t0 = &d->ht[0];
        t1 = &d->ht[1];

        /* Make sure t0 is the smaller and t1 is the bigger table */

        // 确保 t0 比 t1 要小
        if (t0->size > t1->size) {
            t0 = &d->ht[1];
            t1 = &d->ht[0];
        }

        // 记录掩码
        m0 = t0->sizemask;
        m1 = t1->sizemask;

        /* Emit entries at cursor */
        // 指向桶，并迭代桶中的所有节点
        de = t0->table[v & m0];
        while (de) {
            fn(privdata, de);
            de = de->next;
        }

        /* Iterate over indices in larger table that are the expansion
         * of the index pointed to by the cursor in the smaller table */
        // Iterate over indices in larger table             // 迭代大表中的桶
        // that are the expansion of the index pointed to   // 这些桶被索引的 expansion 所指向
        // by the cursor in the smaller table               //
        do {
            /* Emit entries at cursor */
            // 指向桶，并迭代桶中的所有节点
            de = t1->table[v & m1];
            while (de) {
                fn(privdata, de);
                de = de->next;
            }

            /* Increment bits not covered by the smaller mask */
            v = (((v | m0) + 1) & ~m0) | (v & m0);

            /* Continue while bits covered by mask difference is non-zero */
        } while (v & (m0 ^ m1));
    }

    /* Set unmasked bits so incrementing the reversed cursor
     * operates on the masked bits of the smaller table */
    v |= ~m0;

    /* Increment the reverse cursor */
    v = rev(v);
    v++;
    v = rev(v);

    return v;
}

static int _dictExpandIfNeeded(dict *d)
{
    /* Incremental rehashing already in progress. Return. */
    // 渐进式 rehash 已经在进行了，直接返回
    if (dictIsRehashing(d)) return DICT_OK;

    /* If the hash table is empty expand it to the initial size. */
    // 如果字典（的 0 号哈希表）为空，那么创建并返回初始化大小的 0 号哈希表
    // T = O(1)
    if (d->ht[0].size == 0) return dictExpand(d, DICT_HT_INITIAL_SIZE);

    /* If we reached the 1:1 ratio, and we are allowed to resize the hash
     * table (global setting) or we should avoid it but the ratio between
     * elements/buckets is over the "safe" threshold, we resize doubling
     * the number of buckets. */
    // 一下两个条件之一为真时，对字典进行扩展
    // 1）字典已使用节点数和字典大小之间的比率接近 1：1
    //    并且 dict_can_resize 为真
    // 2）已使用节点数和字典大小之间的比率超过 dict_force_resize_ratio
    if (d->ht[0].used >= d->ht[0].size &&
        (dict_can_resize ||
         d->ht[0].used/d->ht[0].size > dict_force_resize_ratio))
    {
        // 新哈希表的大小至少是目前已使用节点数的两倍
        // T = O(N)
        return dictExpand(d, d->ht[0].used*2);
    }

    return DICT_OK;
}

/* Our hash table capability is a power of two */
/*
 * 计算第一个大于等于 size 的 2 的 N 次方，用作哈希表的值
 *
 * T = O(1)
 */
static unsigned long _dictNextPower(unsigned long size)
{
    unsigned long i = DICT_HT_INITIAL_SIZE;

    if (size >= LONG_MAX) return LONG_MAX;
    while(1) {
        if (i >= size)
            return i;
        i *= 2;
    }
}

/* Returns the index of a free slot that can be populated with
 * a hash entry for the given 'key'.
 * If the key already exists, -1 is returned.
 *
 * 返回可以将 key 插入到哈希表的索引位置
 * 如果 key 已经存在于哈希表，那么返回 -1
 *
 * Note that if we are in the process of rehashing the hash table, the
 * index is always returned in the context of the second (new) hash table.
 *
 * 注意，如果字典正在进行 rehash ，那么总是返回 1 号哈希表的索引。
 * 因为在字典进行 rehash 时，新节点总是插入到 1 号哈希表。
 *
 * T = O(N)
 */
static int _dictKeyIndex(dict *d, const void *key)
{
    unsigned int h, idx, table;
    dictEntry *he;

    /* Expand the hash table if needed */
    // 单步 rehash
    // T = O(N)
    if (_dictExpandIfNeeded(d) == DICT_ERR)
        return -1;

    /* Compute the key hash value */
    // 计算 key 的哈希值
    h = dictHashKey(d, key);
    // T = O(1)
    for (table = 0; table <= 1; table++) {

        // 计算索引值
        idx = h & d->ht[table].sizemask;

        /* Search if this slot does not already contain the given key */
        // 查找 key 是否存在
        // T = O(1)
        he = d->ht[table].table[idx];
        while(he) {
            if (dictCompareKeys(d, key, he->key))
                return -1;
            he = he->next;
        }

        // 如果运行到这里时，说明 0 号哈希表中所有节点都不包含 key
        // 如果这时 rehahs 正在进行，那么继续对 1 号哈希表进行 rehash
        if (!dictIsRehashing(d)) break;
    }

    // 返回索引值
    return idx;
}

/*
 * 清空字典上的所有哈希表节点，并重置字典属性
 *
 * T = O(N)
 */
void dictEmpty(dict *d, void(callback)(void*)) {

    // 删除两个哈希表上的所有节点
    // T = O(N)
    _dictClear(d,&d->ht[0],callback);
    _dictClear(d,&d->ht[1],callback);
    // 重置属性
    d->rehashidx = -1;
    d->iterators = 0;
}

/*
 * 开启自动 rehash
 *
 * T = O(1)
 */
void dictEnableResize(void) {
    dict_can_resize = 1;
}

/*
 * 关闭自动 rehash
 *
 * T = O(1)
 */
void dictDisableResize(void) {
    dict_can_resize = 0;
}
