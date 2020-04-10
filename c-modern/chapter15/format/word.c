//
// Created by 盆栽 on 2020/1/31.
//
#include <stdio.h>
#include "word.h"

static int read_char(void) {
    int ch = getchar();

    if (ch == '\n' || ch == '\t')
        return ' ';
    return ch;
}


void read_word(char *word, int len) {
    int ch, pos = 0;

    while ( (ch = read_char()) == ' ') {}

    while ( ch != ' ' && ch != EOF ) {
        if (pos < len)
            word[pos++] = ch;
        ch = read_char();
        word[pos] = '\0';
    }
}
