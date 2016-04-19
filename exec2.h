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

int pipes_amount(char ** argv, int argc)
{
    int i, ans = 0;
    for (i = 0; i < argc; ++i) {
        ans += (strlen(argv[i]) == 1 && argv[i][0] == '|');
    }
    return ans;
}

void find_pipes(char ** argv, int argc, int * pipe_begins) {
    int i, k;
    for (i = 0, k = 0; i < argc; ++i) {
        if (strlen(argv[i]) == 1 && argv[i][0] == '|') {
            pipe_begins[k++] = i;
        }
    }
}

/*void stream_processing(argv, argc) {

}*/

int exec_function(int argc, char ** argv)
{
    int i = 0;
    /*for (i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
    }
    printf("-- exec_func\n");*/
    /*printf("forked\n");*/
    /*int parts = pipes_amount(argv, argc) + 1;
    int * pipe_begins = (int *)malloc(sizeof(int) * (parts));
    find_pipes(argv, argc, pipe_begins);
    int pipe_;
    for (pipe_ = 0; pipe_ < parts; ++pipe_) {*/
    int last = 1;
    int pipes_num = pipes_amount(argv, argc) + 1;
    int pipe_descs[pipes_num][2];
    pipe_descs[0][0] = 0; pipe_descs[0][1] = 1;
    int current_pipe = 0;
    int status;
    int p = 1; /* 0 if we are child process */
    for (i = 0; i < argc; ++i) {
        int len = strlen(argv[i]);
        if (strcmp(argv[i], "|") == 0 || i == argc - 1) {
            if (p == 0) {
                if (i != argc - 1)
                    argv[i] = (char *)0;
                fprintf(stderr, "\tstarting %s\n", argv[last]);
                status = execvp(argv[last], argv + last);
                fprintf(stderr, "\tend of %s\n", argv[last]);
                close(1);
                close(0);
                exit(status);
            }
        }
        if (strcmp(argv[i], "|") == 0 || i == 0) {
            if (p > 0) {
                ++current_pipe;
                if (current_pipe < pipes_num) {
                    printf("pipe\n");
                    status = pipe(pipe_descs[current_pipe]);
                    if (status == -1) {
                        fprintf(stderr, "cannot create a pipe\n");
                    }
                }
                p = fork();
                if (p == 0) {
                    printf("forked\n");
                    if (current_pipe < pipes_num) {
                        printf("copying %d to %d\n",pipe_descs[current_pipe][1], 1);
                        close(1);
                        dup2(pipe_descs[current_pipe][1], 1);
                    }
                    if (current_pipe != 1) {
                        close(0);
                        printf("copying %d to %d\n",pipe_descs[current_pipe - 1][0], 0);
                        dup2(pipe_descs[current_pipe - 1][0], 0);
                    }
                } else if (p > 0) {
                    printf("\twaiting for %d\n", p);
                    waitpid(p, &status, 0);
                    if (current_pipe < pipes_num) {
                        fprintf(stderr, "closing %d\n", pipe_descs[current_pipe][1]);
                        close(pipe_descs[current_pipe][1]);
                    }
                    if (current_pipe != 1) {
                        fprintf(stderr, "closing %d\n", pipe_descs[current_pipe - 1][0]);
                        close(pipe_descs[current_pipe - 1][0]);
                    }
                    printf("\tsuccess\n");
                }
            }
            last = i + 1;
        }
        if ((len == 2 && is_gl_than_sign(argv[i][1])) || (len == 1 && is_gl_than_sign(argv[i][0]))) {
            if (p == 0) {
                int stream = 0;
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
    }
    /*}
    printf("exit_code=%d\n", ret);
    if (ret != 0) {
        exit(ret);
    }
    return ret;*/
    return status;
}
