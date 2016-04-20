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


int exec_function(int argc, char ** argv)
{
    int i = 0;
    for (i = 0; i < argc; ++i) {
        printf("'%s'\n", argv[i]);
    }
    int last = 1;
    int pipes_num = pipes_amount(argv, argc);
    int pipe_descs[pipes_num][2];
    int processes_id[pipes_num];
    int proc_id_pos = 0;
    int current_pipe = 0;
    int status;
    int p = 1; /* 0 if we are child process */
    for (i = 0; i < argc; ++i) {
        int len = strlen(argv[i]);
        if (strcmp(argv[i], "|") == 0 || i == argc - 1) {
            if (p == 0) {
                if (i != argc - 1)
                    argv[i] = (char *)0;
                /*fprintf(stderr, "\tstarting %s\n", argv[last]);*/
                int j; /* we must close all open pipes to write */
                for (j = 0; j <= current_pipe - 2; ++j) {
                    /*fprintf(stderr, "\tclosing %d in child\n", pipe_descs[j][1]);*/
                    close(pipe_descs[j][1]);
                }
                status = execvp(argv[last], argv + last);
                exit(status); /* if cannot exec */
            }
        }
        if (strcmp(argv[i], "|") == 0 || i == 0) {
            if (p > 0) {
                if (current_pipe < pipes_num) {
                    status = pipe(pipe_descs[current_pipe]);
                    /*printf("\tcreating pipe (%d, %d) in %d\n", pipe_descs[current_pipe][0], pipe_descs[current_pipe][1], p);*/
                    if (status == -1) {
                        fprintf(stderr, "cannot create a pipe\n");
                    }
                }
                ++current_pipe;
                p = fork();
                if (p == 0) {
                    /*printf("forked\n");*/
                    if (current_pipe - 1 < pipes_num) {
                        /*printf("copying %d to %d\n",pipe_descs[current_pipe - 1][1], 1);*/
                        close(1);
                        dup2(pipe_descs[current_pipe - 1][1], 1);
                    }
                    if (current_pipe > 1) {
                        close(0);
                        /*printf("copying %d to %d\n",pipe_descs[current_pipe - 2][0], 0);*/
                        dup2(pipe_descs[current_pipe - 2][0], 0);
                    }
                } else if (p > 0) {
                    processes_id[proc_id_pos++] = p;
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
                /*printf("redirect_stream %d to %s\n", stream, argv[i + 1]);*/
                redirect_stream(argv[i + 1], stream);
            }
        }
    }
    for (i = 0; i < proc_id_pos; ++i) {
        /*fprintf(stderr, "\twaiting for %d\n", processes_id[i]);*/
        waitpid(processes_id[i], &status, 0);
        /*fprintf(stderr, "\t%d is terminated\n", processes_id[i]);*/
        if (i < pipes_num) {
            /*fprintf(stderr, "\tclosing %d\n", pipe_descs[i][1]);*/
            close(pipe_descs[i][1]);
        }
        if (i != 0) {
            /*fprintf(stderr, "\tclosing %d\n", pipe_descs[i - 1][0]);*/
            close(pipe_descs[i - 1][0]);
        }
        /*fprintf(stderr, "\tend waiting %d\n", processes_id[i]);*/
    }
    return status;
}
