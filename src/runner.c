#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "dbg.h"
#include "runner.h"

int Runner_run(char *path, char *cwd, char *argv[])
{
    chdir(cwd);
    return execv(path, argv);
}

int Runner_runfn(char *fn, char *cwd, int argc, char *argv[])
{
    char *tmp;
    char *cmd;

    asprintf(&cmd, "%s%s $@", config.cmd_prefix, fn);
    check(cmd != NULL, "failed to function name");

    char *eargv[] = { "bash", "--rcfile", "Shakefile", "-i",
                      "-c",   cmd,        "Shakefile", NULL };

    int eargc = sizeof(eargv) / sizeof(char *);
    int rargc = argc - 1;

    char **xargv = malloc((eargc + rargc) * sizeof(char *));
    check_mem(xargv);

    tmp = memcpy(&xargv[0], &eargv[0], eargc * sizeof(char *));
    check_mem(tmp);
    memcpy(&xargv[eargc - 1], &argv[1], rargc * sizeof(char *));
    check_mem(tmp);

    chdir(cwd);
    return execvp("bash", xargv);

error:
    return -1;
}