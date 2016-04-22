#ifndef COMMAND_DESCRIPTION_H
#define COMMAND_DESCRIPTION_H

#include <stdio.h>
#include <stdlib.h>

typedef int (*command_type)(int argc, char ** argv);

#define MAX_POSSIBLE_COMMAND_SIZE 15

struct Command
{
    char * name;
    command_type command;
};


#endif
