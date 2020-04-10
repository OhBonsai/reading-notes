//
// Created by 盆栽 on 2020/1/31.
//
#include <string.h>
#include "line.h"
#include "word.h"


#define MAX_WORD_LEN 20

int main(void) {
    char word[MAX_WORD_LEN+1];
    int word_len;

    clear_line();

    for (int i=0 ; i< 100; i++) {
        read_word(word, MAX_WORD_LEN);
        word_len = strlen(word);

        if (word_len == 0) {
            flush_line();
            return 0;
        }

        if (word_len > MAX_WORD_LEN)
            word[MAX_WORD_LEN] = '*';

        if (word_len + 1 > space_remaining()) {
            write_line();
            clear_line();
        }

        add_word(word);
    }
}
