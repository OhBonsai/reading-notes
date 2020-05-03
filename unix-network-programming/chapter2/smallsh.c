//
// Created by 盆栽 on 2020/5/2.
//

#include "smallsh.h"
#include <stdio.h>

#define MAXBUF 9999
#define EOL 1
#define ARG 2
#define AMPERSAND 3
#define SEMICOLON 4

#define MAXARG 100
#define FOREGROUND 0
#define BACKGROUND 1


static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr=inpbuf, *tok=tokbuf;

int userin(char *p) {
    int c, count;
    ptr = inpbuf;
    tok = tokbuf;

    printf("%s", p);

    for (count =0;;) {
        if ( c=getchar() == EOF) return EOF;
        if (count <MAXBUF) inpbuf[count++] = c;
        if(c=='\n' && count <MAXBUF) {
            inpbuf[count] = '\0';
            return count;
        }
        if (c=='\n') {
            printf("smallsh: input line to long \n");
            count =0;
            printf("%s", p);
        }

    }

}

void gettok(char *output) {
    int type;
    char *outptr = tok;

    for (; *ptr == '"'|| *ptr == '\t'; ptr++) {

    }


}