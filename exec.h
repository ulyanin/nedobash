#include <stdio.h>
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

int redirect_stream(char * fname, int stream, char ** argv)
{
    int p = fork();
    if (p == 0) {
        int mode = 0;
        if (stream == 0) {
            mode = O_RDONLY;
        } else if (stream == 1) {
            mode = O_WRONLY;
        } else if (stream == 2) {
            mode = O_WRONLY;
        }
        int fdesk = open(fname, mode);
        close(stream);
        dup2(fdesk, stream);
        return execvp(argv[0], argv);
    }
    return 0;
}

int exec_function(int argc, char ** argv)
{
    int i = 0;
    for (i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
    }
    printf("-- exec_func\n");
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
            redirect_stream(argv[i + 1], stream, argv);
        }
    }
    /*int p = fork();
    if (p == 0) {
        int ret = execvp(argv[1], argv+1);
        return ret;
    }
    int status = 0;
    waitpid(p, &status, 0);*/
    return 0;
}
