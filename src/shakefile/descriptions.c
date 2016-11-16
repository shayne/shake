//
// Created by shayne on 11/15/16.
//

#include "../config.h"
#include "../dbg.h"
#include "shakefile.h"
#include <bstrlib.h>
#include <stdlib.h> // needed?
#include <sys/wait.h>
#include <unistd.h>

int Shakefile_detect_descriptions(size_t size, char **descs)
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
               config.proj_file,
               "declare",
               "-f",
               NULL);
        perror("execlp");
        _exit(1);
    }

    close(pipes[1]);

    int i = 0;
    int pos;
    bstring line;
    FILE *stdout = fdopen(pipes[0], "r");
    struct tagbstring parens = bsStatic("()");
    bstring prefix = bformat("%s", config.cmd_prefix);
    while ((line = bgets((bNgetc)fgetc, stdout, '\n')) != NULL) {
        btrimws(line);

        pos = binstrcaseless(line, 0, prefix);
        if (pos != 0)
            goto end;

        pos = binstr(line, prefix->slen + 1, &parens);
        if (pos != (line->slen - 2))
            goto end;

        bstring nextline;
        nextline = bgets((bNgetc)fgetc, stdout, '\n');
        bdestroy(nextline);
        nextline = bgets((bNgetc)fgetc, stdout, '\n');

        btrimws(nextline);
        if (bchar(nextline, 0) != ':') {
            bdestroy(nextline);
            i++; // this was a fn so increment the index
            goto end;
        }

        bdelete(nextline, 0, 1); // remove colon
        btrimws(nextline);
        bdelete(nextline, nextline->slen - 1, 1); // remove trailing semi-colon

        descs[i++] = bstr2cstr(nextline, '\0');
        bdestroy(nextline);

    end:
        bdestroy(line);
    }

    bdestroy(prefix);
    fclose(stdout);
    wait(0);

    return i;

error:
    return -1;
}
