#include "help.h"
#include "command_adder.h"

extern struct Command * commands;
extern int commands_amount;

int help_function(int argc, char ** argv)
{
    printf("Hello! Welcome to nedobash -- simply powerfull bash realisation on pure c\n");
    printf("You can run your command with exec\n");
    printf("For example:\n");
    printf("\texec echo azaza > file.txt\n");
    printf("\texec cat file.txt | sort | uniq > sorted.txt\n");
    printf("You can also load your own plugin (see an exapmle in bc.c)\n");
    printf("To load your .so plugin type:\n");
    printf("\tload_plugin ./YOUR_LIB.so\n");
    printf("list of supported commands\n");
    int i;
    for (i = 0; i < commands_amount; ++i) {
        printf("#%d:%s\n", i, commands[i].name);
    }
    return 0;
}

