#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dbg.h"
#include "runner.h"

int Runner_run(char *script_name, char *cwd, char *argv[])
{
    char *cmd;
    asprintf(&cmd, "scripts/run-%s.sh", script_name);
    check(cmd != NULL, "failed to screate script");

    chdir(cwd);
    return execv(cmd, argv);

error:
    return -1;
}

int Runner_runfn(char *fn, char *cwd, int argc, char *argv[])
{
    char *cmd;
    asprintf(&cmd, "cmd-%s $@", fn);
    check(cmd != NULL, "failed to function name");

    char *eargv[] = { "bash", "--rcfile", "Shakefile", "-i",
                      "-c",   cmd,        "Shakefile", NULL };

    int eargc = sizeof(eargv) / sizeof(char *);
    int rargc = argc - 1;

    //        char **xargv = calloc(eargc + rargc, sizeof(char *));
    char *xargv[255]; // = { 0 };

    memcpy(&xargv[0], &eargv[0], eargc * sizeof(char *));
    memcpy(&xargv[eargc - 1], &argv[1], rargc * sizeof(char *));
    chdir(cwd);
    return execvp("bash", xargv);

error:
    return -1;
}