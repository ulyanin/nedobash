#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int is_gl_than_sign(char c)
{
    return c == '<' || c == '>';
}

void redirect_stream(char * fname, int stream)
{
    int mode = 0;
    int flags = 0;
    if (stream == 0) {
        flags = O_RDONLY;
    } else if (stream == 1) {
        flags = O_WRONLY | O_CREAT;
        mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    } else if (stream == 2) {
        flags = O_WRONLY | O_CREAT;
        mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    int fdesk = open(fname, flags, mode);
    close(stream);
    dup2(fdesk, stream);
    /*close(fdesk);*/
}


int exec_function(int argc, char ** argv)
{
    int i = 0;
    /*for (i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
    }
    printf("-- exec_func\n");*/
    int p = fork();
    if (p == 0) {
        /*printf("forked\n");*/
        for (i = 0; i < argc; ++i) {
            int len = strlen(argv[i]);
            int stream = 0;
            if ((len > 1 && is_gl_than_sign(argv[i][1])) || (len == 1 && is_gl_than_sign(argv[i][0]))) {
                if (len == 1) {
                    stream = (argv[i][0] == '>');
                } else {
                    stream = argv[i][0] - '0';
                }
                if (stream < 0 || stream > 2 || i == argc - 1)
                    continue;
                argv[i] = (char *)0;
                /*fprintf(stdout, "redirectinc stream\n");*/
                redirect_stream(argv[i + 1], stream);
                /*fprinf(stdout, "completed\n");*/
            }
        }
        exit(execvp(argv[1], argv+1));
        /*printf("exit_code=%d\n", ret);
        if (ret != 0) {
            exit(ret);
        }
        return ret;*/
    }
    int status = 0;
    waitpid(p, &status, 0);
    return status;
}
