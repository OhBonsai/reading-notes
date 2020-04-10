//
// Created by 盆栽 on 2020/1/30.
//
#include <stdio.h>

#define MAX_LENGTH 100

static void get_word(char* result[]) {
    char y = 'y';
    int i = 0;

    while ((y == 'y') && (i < MAX_LENGTH)) {
        char t[80];
        printf("\nEnter word: ");
        scanf("%s", t);
        result[i] = t;
        i++;

        printf("\nWanna continue[Y/n]: ");
        fflush(stdin);
        scanf("%c", &y);
        scanf("%c", &y);
    }
}


void main1() {
    char* result[MAX_LENGTH];
    get_word(result);
    int max=0, min=0;
    char* max_str;
    char* min_str;

    for (int i=0; i<MAX_LENGTH; i++) {
        if (result[i] == NULL) break;

        if (i == 0) {
            max_str = min_str = result[i];

        }

        size_t len = strlen(result[i]);

        if (len > max) {
            max = len;
            max_str = result[i];
        }

        if (len < min) {
            min = len;
            min_str = result[i];
        }
    }

    printf("Smallest word: %s\n", min_str);
    printf("Largest word: %s\n", max_str);
}
