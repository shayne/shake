//
// Created by shayne on 11/7/16.
//

#include <bstrlib.h>
#include <stdlib.h>  // needed?
#include <sys/wait.h>
#include <unistd.h>
#include "../config.h"
#include "../dbg.h"
#include "shakefile.h"

int Shakefile_detect_functions(size_t size, char *fns[]) {
    pid_t pid;
    int pipes[2];

    check(pipe(pipes) == 0, "Failed to create pipes");

    if ((pid = fork()) == -1) sentinel("pipe failed on fork");

    if (pid == 0) {
        while ((dup2(pipes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {
        }
        close(pipes[0]);
        close(pipes[1]);
        execlp("bash", "bash", "-c", "source $0 && $@", config.proj_file,
               "compgen", "-A", "function", config.cmd_prefix, NULL);
        perror("execlp");
        _exit(1);
    }

    close(pipes[1]);

    int i = 0;
    bstring line;
    FILE *stdout = fdopen(pipes[0], "r");
    while ((line = bgets((bNgetc)fgetc, stdout, '\n')) != NULL) {
        btrimws(line);
        bdelete(line, 0, config.cmd_prefix_len);

        char *fn = bdata(line);
        check(fn != NULL, "bdata line was null.");

        fns[i++] = strdup(fn);
        check(i < (int)size, "too many functions");

        bdestroy(line);
    }

    fclose(stdout);
    wait(0);

    return i;

error:
    return -1;
}

int Shakefile_has_fn(char *name)  // TODO: change 0 ret as success
{
    int i;
    int rb = 0;  // 0 for false
    int fncount = 0;
    char *fns[255];

    fncount = Shakefile_detect_functions(255, fns);

    for (i = 0; i < fncount; i++) {
        if (strcmp(fns[i], name) == 0) {
            rb = 1;
            break;
        }
    }

    for (i = 0; i < fncount; i++) free(fns[i]);

    return rb;
}
