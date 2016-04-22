#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "plugin_load.h"
#include "command_parser.h"
#include "command_adder.h"
#include "command_description.h"
#include "exec.h"
#include "help.h"

const int MAX_CMD_SIZE = 1000;

struct Command * commands = NULL;
int commands_amount = 0;


void add_base_commands()
{
    struct Command exec_cmd = {"exec", exec_function};
    add_command(exec_cmd);
    struct Command load_cmd = {"load_plugin", plugin_load};
    add_command(load_cmd);
    struct Command help_cmd = {"help", help_function};
    add_command(help_cmd);
}

int init()
{
    add_base_commands();
    while (1) {
        int words_amount;
        printf("nedobash>>> ");
        char ** cmd = readCommand(&words_amount, MAX_CMD_SIZE);
        if (words_amount == -1) {
            printf("exit\n");
            break;
        }
        if (cmd == NULL) {
            printf("too large command size\n");
            printf("be carefull it may cause undefined behaviour\n");
            /* fflush(stdin);*/
            continue;
        }
        int i;
        for (i = 0; i < commands_amount; ++i) {
            if (strcmp(commands[i].name, cmd[0]) == 0) {
                int ret = commands[i].command(words_amount, cmd);
                if (ret != 0) {
                    printf("Sorry, I can't do this :(\n");
                    printf("%d ", ret);
                }
            }
        }
        /*for (i = 0; i < words_amount; ++i) {
            printf("'%s'\n", cmd[i]);
        }*/
        for (i = 0; i < words_amount; ++i) {
            free(cmd[i]);
        }
        free(cmd);
        cmd = NULL;
        words_amount = 0;
    }
    return 0;
}

int main()
{
    return init();
}
