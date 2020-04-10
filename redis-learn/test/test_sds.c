//
// Created by 盆栽 on 2020/3/4.
//
#include "../sds.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void test_sds_trim() {
    sds a = sdsnew("abc");

    assert(sdstrim(a, "c")[0] == 'a');
    assert(sdstrim(a, "c")[1] == 'b');

}

int main(int argc, char **argv) {
    test_sds_trim();

    return 0;
}