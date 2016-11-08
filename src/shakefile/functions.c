//
// Created by shayne on 11/7/16.
//

#include "../dbg.h"
#include "shakefile.h"
#include <bstrlib.h>
#include <sys/wait.h>
#include <unistd.h>

void Shakefile_detect_functions(char *projfile, CharPArray *cap)
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

    bstring line;
    FILE *stdout = fdopen(pipes[0], "r");
    while ((line = bgets((bNgetc)fgetc, stdout, '\n')) != NULL) {
        btrimws(line);
        bdelete(line, 0, 4); // remove "cmd-"
        capinsert(cap, bdata(line), (size_t)blength(line));
        bdestroy(line);
    }

    fclose(stdout);
    wait(0);

error:
    return;
}
