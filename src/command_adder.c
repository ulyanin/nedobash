#include "command_adder.h"

extern struct Command * commands;
extern int commands_amount;

void add_command(struct Command cmd)
{
    commands_amount++;
    commands = realloc(commands, commands_amount * sizeof(struct Command));
    commands[commands_amount - 1] = cmd;
}
