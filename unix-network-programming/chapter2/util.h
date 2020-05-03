//
// Created by 盆栽 on 2020/5/2.
//

#ifndef UNIXNETWORK_UTIL_H
#define UNIXNETWORK_UTIL_H


#include <stdlib.h>
failure(char *s) {
    perror(s);
    exit(1);
}


printpos(char*string, int filders) {
    long pos = lseek(filders, 0L, SEEK_CUR);
    if (pos < 0L) {
        failure("lseek failed");
    }


    printf("%s: %ld\n", string , pos);
}

#endif //UNIXNETWORK_UTIL_H
