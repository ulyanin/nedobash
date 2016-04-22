#include <stdio.h>
#include "command_description.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int sum_length(int argc, char ** argv) {
    int sum = 0, i;
    for (i = 0; i < argc; ++i)
        sum += strlen(argv[i]);
    return sum;
}

int bc(int argc, char ** argv)
{
    char echo[] = "echo ";
    char bc_calc[] = "|bc";
    char * expr = (char *)malloc(sizeof(char) * (sum_length(argc - 1, argv + 1) + 1 + strlen(echo) + strlen(bc_calc)));
    strcpy(expr, echo);
    for (int i = 1; i < argc; ++i)
        strcat(expr, argv[i]);
    strcat(expr, bc_calc);
    printf("expr=%s\n", expr);
    return system(expr);
}

struct Command init()
{
    struct Command bc_cmd = {"bc", bc};
    return bc_cmd;
}
