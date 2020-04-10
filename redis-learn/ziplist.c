//
// Created by 盆栽 on 2020/3/22.
//

#include "ziplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ZIP_END 255
#define ZIP_BIGLEN 254


#define ZIP_STR_MASK 0xc0
#define ZPI_INT_MASK 0x30

#define ZIP_STR_06B (0 << 6)
#define ZIP_STR_14B (1 << 6)
#define ZIP_STR_32B (2 << 6)


#define ZIP_INT_16B (0xc0 | 0<<4)
#define ZIP_INT_32B (0xc0 | 1<<4)
#define ZIP_INT_64B (0xc0 | 2<<4)
#define ZIP_INT_24B (0xc0 | 3<<4)
#define ZIP_INT_8B 0xfe


#define ZIP_INT_IMM_MASK 0x0f
#define ZIP_INT_IMM_MIN 0xf1    /* 11110001 */
#define ZIP_INT_IMM_MAX 0xfd    /* 11111101 */
#define ZIP_INT_IMM_VAL(v) (v & ZIP_INT_IMM_MASK)


#define INT24_MAX 0x7fffff
#define INT24_MIN (-INT24_MAX - 1)
#define ZIP_IS_STR(enc) (((enc) & ZIP_STR_MASK) < ZIP_STR_MASK)

#define ZIPLIST_BYTES(zl)       (*((uint32_t*)(zl)))
#define ZIPLIST_TAIL_OFFSET(zl) (*((uint32_t*)((zl)+sizeof(uint32_t))))
#define ZIPLIST_LENGTH(zl)      (*((uint16_t*)((zl)+sizeof(uint32_t)*2)))
#define ZIPLIST_HEADER_SIZE     (sizeof(uint32_t)*2+sizeof(uint16_t))
#define ZIPLIST_ENTRY_HEAD(zl)  ((zl)+ZIPLIST_HEADER_SIZE)
#define ZIPLIST_ENTRY_TAIL(zl)  ((zl)+ZIPLIST_TAIL_OFFSET(zl))
#define ZIPLIST_ENTRY_END(zl)   ((zl)+ZIPLIST_BYTES(zl)-1)


#define ZIPLIST_INCR_LENGTH(zl, incr) {\
    if (ZIPLIST_LENGHT(zl) < UINT16_MAX) \
    ZIPLIST_LENGTH(zl) = ZIPLIST_LENGTH(zl)+incr; \
}


#define ZIP_ENTRY_ENCODING(ptr, encoding) do { \
    (encoding) = (ptr[0]); \
    if ((encoding) < ZIP_STR_MASK) (encoding) &= ZIP_STR_MASK; \
} while(0)


static unsigned int zipIntSize(unsigned char encoding) {
    switch(encoding) {
        case ZIP_INT_8B:  return 1;
        case ZIP_INT_16B: return 2;
        case ZIP_INT_24B: return 3;
        case ZIP_INT_32B: return 4;
        case ZIP_INT_64B: return 8;
        default: return 0; /* 4 bit immediate */
    }
}


static unsigned int zipEncodeLength(unsigned char *p, unsigned char encoding, unsigned int rawlen) {
    unsigned char len = 1, buf[5];

    if (ZIP_IS_STR(encoding)) {
        if (rawlen <= 0x3f) {
            if (!p) return len;
            buf[0] = ZIP_STR_06B | rawlen;
        } else if (rawlen <= 0x3fff) {
            len += 1;
            if (!p) return len;
            buf[0] = ZIP_STR_14B | ((rawlen >> 8) & 0x3f);
            buf[1] = rawlen & 0xff;
        } else {
            len += 4;
            if (!p) return len;
            buf[0] = ZIP_STR_32B;
            buf[1] = (rawlen >> 24) & 0xff;
            buf[2] = (rawlen >> 16) & 0xff;
            buf[3] = (rawlen >> 8) & 0xff;
            buf[4] = rawlen & 0xff;
        }
    } else {
        /* Implies integer encoding, so length is always 1. */
        if (!p) return len;
        buf[0] = encoding;
    }

    memcpy(p,buf,len);
    return len;
}

#define ZIP_DECODE_LENGTH(ptr, encoding, lensize, len) do {                    \
                                                                               \
    /* 取出值的编码类型 */                                                     \
    ZIP_ENTRY_ENCODING((ptr), (encoding));                                     \
                                                                               \
    /* 字符串编码 */                                                           \
    if ((encoding) < ZIP_STR_MASK) {                                           \
        if ((encoding) == ZIP_STR_06B) {                                       \
            (lensize) = 1;                                                     \
            (len) = (ptr)[0] & 0x3f;                                           \
        } else if ((encoding) == ZIP_STR_14B) {                                \
            (lensize) = 2;                                                     \
            (len) = (((ptr)[0] & 0x3f) << 8) | (ptr)[1];                       \
        } else if (encoding == ZIP_STR_32B) {                                  \
            (lensize) = 5;                                                     \
            (len) = ((ptr)[1] << 24) |                                         \
                    ((ptr)[2] << 16) |                                         \
                    ((ptr)[3] <<  8) |                                         \
                    ((ptr)[4]);                                                \
        } else {                                                               \
        }                                                                      \
                                                                               \
    /* 整数编码 */                                                             \
    } else {                                                                   \
        (lensize) = 1;                                                         \
        (len) = zipIntSize(encoding);                                          \
    }                                                                          \
} while(0);


static unsigned int zipPrevEncodeLength(unsigned char *p, unsigned int len) {

    // 仅返回编码 len 所需的字节数量
    if (p == NULL) {
        return (len < ZIP_BIGLEN) ? 1 : sizeof(len)+1;

        // 写入并返回编码 len 所需的字节数量
    } else {

        // 1 字节
        if (len < ZIP_BIGLEN) {
            p[0] = len;
            return 1;

            // 5 字节
        } else {
            // 添加 5 字节长度标识
            p[0] = ZIP_BIGLEN;
            // 写入编码
            memcpy(p+1,&len,sizeof(len));
            // 如果有必要的话，进行大小端转换
            // 返回编码长度
            return 1+sizeof(len);
        }
    }
}

static void zipPrevEncodeLengthForceLarge(unsigned char *p, unsigned int len) {

    if (p == NULL) return;

    // 设置 5 字节长度标识
    p[0] = ZIP_BIGLEN;

    // 写入 len
    memcpy(p+1,&len,sizeof(len));
}

#define ZIP_DECODE_PREVLENSIZE(ptr, prevlensize) do {                          \
    if ((ptr)[0] < ZIP_BIGLEN) {                                               \
        (prevlensize) = 1;                                                     \
    } else {                                                                   \
        (prevlensize) = 5;                                                     \
    }                                                                          \
} while(0);


#define ZIP_DECODE_PREVLEN(ptr, prevlensize, prevlen) do {                     \
                                                                               \
    /* 先计算被编码长度值的字节数 */                                           \
    ZIP_DECODE_PREVLENSIZE(ptr, prevlensize);                                  \
                                                                               \
    /* 再根据编码字节数来取出长度值 */                                         \
    if ((prevlensize) == 1) {                                                  \
        (prevlen) = (ptr)[0];                                                  \
    } else if ((prevlensize) == 5) {                                           \
        assert(sizeof((prevlensize)) == 4);                                    \
        memcpy(&(prevlen), ((char*)(ptr)) + 1, 4);                             \
    }                                                                          \
} while(0);


static int zipPrevLenByteDiff(unsigned char *p, unsigned int len) {
    unsigned int prevlensize;

    // 取出编码原来的前置节点长度所需的字节数
    // T = O(1)
    ZIP_DECODE_PREVLENSIZE(p, prevlensize);

    // 计算编码 len 所需的字节数，然后进行减法运算
    // T = O(1)
    return zipPrevEncodeLength(NULL, len) - prevlensize;
}


static unsigned int zipRawEntryLength(unsigned char *p) {
    unsigned int prevlensize, encoding, lensize, len;

    // 取出编码前置节点的长度所需的字节数
    // T = O(1)
    ZIP_DECODE_PREVLENSIZE(p, prevlensize);

    // 取出当前节点值的编码类型，编码节点值长度所需的字节数，以及节点值的长度
    // T = O(1)
    ZIP_DECODE_LENGTH(p + prevlensize, encoding, lensize, len);

    // 计算节点占用的字节数总和
    return prevlensize + lensize + len;
}

static int zipTryEncoding(unsigned char *entry, unsigned int entrylen, long long *v, unsigned char *encoding) {
    long long value;

    // 忽略太长或太短的字符串
    if (entrylen >= 32 || entrylen == 0) return 0;

    // 尝试转换
    // T = O(N)
    if (string2ll((char*)entry,entrylen,&value)) {

        /* Great, the string can be encoded. Check what's the smallest
         * of our encoding types that can hold this value. */
        // 转换成功，以从小到大的顺序检查适合值 value 的编码方式
        if (value >= 0 && value <= 12) {
            *encoding = ZIP_INT_IMM_MIN+value;
        } else if (value >= INT8_MIN && value <= INT8_MAX) {
            *encoding = ZIP_INT_8B;
        } else if (value >= INT16_MIN && value <= INT16_MAX) {
            *encoding = ZIP_INT_16B;
        } else if (value >= INT24_MIN && value <= INT24_MAX) {
            *encoding = ZIP_INT_24B;
        } else if (value >= INT32_MIN && value <= INT32_MAX) {
            *encoding = ZIP_INT_32B;
        } else {
            *encoding = ZIP_INT_64B;
        }

        // 记录值到指针
        *v = value;

        // 返回转换成功标识
        return 1;
    }

    // 转换失败
    return 0;
}

static void zipSaveInteger(unsigned char *p, int64_t value, unsigned char encoding) {
    int
}