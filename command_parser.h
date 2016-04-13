#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int num_words(char * str)
{
    int ans = 0;
    char * c;
    for (c = str; *c != '\0'; ++c) {
        ans += (!!isspace(*c)); /* convert int to {0, 1} */
        if (c != str && isspace(*(c - 1)) && isspace(*c))
            --ans;
    }
    return ans;
}

char ** readCommand(int * words_amount, int num)
{
    char * buffer = (char *)malloc(sizeof(char) * num);
    if (fgets(buffer, num, stdin) == NULL) {
        *words_amount = -1;
        return NULL;
    }
    int n = strlen(buffer);
    /*printf("'%s'\n", buffer);*/
    *words_amount = num_words(buffer);
    if (n != 0 && buffer[n - 1] != '\n') {
        /* '\n' is not the last symbol =>
         * => string is not read to the end =>
         * => bad read */
        free(buffer);
        *words_amount = 0;
        return NULL;
    }
    char ** words = (char **)malloc(sizeof(char *) * (*words_amount + 1));
    int i, row, j, size;
    for (i = 0; i < *words_amount; ++i)
        words[i] = NULL;
    words[*words_amount] = (char *)0;
    for (i = row = j = size = 0; i < n; ++i) {
        if (i > 0 && isspace(buffer[i]) && isspace(buffer[i - 1]))
            continue;
        if (j == size) {
            size = size * 2 + 1;
            words[row] = (char *)realloc(words[row], sizeof(char) * size);
        }
        if (isspace(buffer[i])) {
            words[row++][j] = 0;
            size = j = 0;
        } else {
            words[row][j++] = buffer[i];
        }
    }
    free(buffer);
    return words;
}

