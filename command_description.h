#include <stdio.h>
#include <stdlib.h>

typedef int (*command_type)(int argc, char ** argv);

#define MAX_POSSIBLE_COMMAND_SIZE 15

struct Command
{
    char name[MAX_POSSIBLE_COMMAND_SIZE];
    command_type command;
};

struct Command * add_command(struct Command * commands, int *commands_amount, struct Command cmd)
{
    (*commands_amount)++;
    commands = realloc(commands, (*commands_amount) * sizeof(struct Command));
    commands[(*commands_amount) - 1] = cmd;
    return commands;
}
