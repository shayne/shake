//
// Created by shayne on 11/7/16.
//

#include "../dbg.h"
#include "shakefile.h"
#include <bstrlib.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int Shakefile_detect_functions(char *projfile, size_t size, char *fns[])
{
    pid_t pid;
    int pipes[2];

    check(pipe(pipes) == 0, "Failed to create pipes");

    if ((pid = fork()) == -1)
        sentinel("pipe failed on fork");

    if (pid == 0) {
        while ((dup2(pipes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {
        }
        close(pipes[0]);
        close(pipes[1]);
        execlp("bash",
               "bash",
               "-c",
               "source $0 && $@",
               projfile,
               "compgen",
               "-A",
               "function",
               NULL);
        perror("execlp");
        _exit(1);
    }

    close(pipes[1]);

    int i = 0;
    bstring line;
    FILE *stdout = fdopen(pipes[0], "r");
    while ((line = bgets((bNgetc)fgetc, stdout, '\n')) != NULL) {
        btrimws(line);
        bdelete(line, 0, 4); // remove "cmd-"

        fns[i++] = strdup(bdata(line));
        check(i < size, "too many functions");

        bdestroy(line);
    }

    fclose(stdout);
    wait(0);

    return i;

error:
    return -1;
}

int Shakefile_has_fn(char *name, char *projfile)
{
    int i;
    int rb = 0; // 0 for false
    int fncount = 0;
    char *fns[255];

    fncount = Shakefile_detect_functions(projfile, 255, fns);

    for (i = 0; i < fncount; i++) {
        if (strcmp(fns[i], name) == 0) {
            rb = 1;
            break;
        }
    }

    for (i = 0; i < fncount; i++)
        free(fns[i]);

    return rb;
}
