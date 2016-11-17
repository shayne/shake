#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "dbg.h"
#include "runner.h"

int Runner_run(char *path, char *cwd, char *argv[])
{
    int rc;
    rc = chdir(cwd);
    check(rc == 0, "chdir failed");
    return execv(path, argv);
error:
    return rc;
}

int Runner_runfn(char *fn, char *cwd, int argc, char *argv[])
{
    int rc;
    char *tmp;
    char *cmd;

    rc = asprintf(&cmd, "%s%s $@", config.cmd_prefix, fn);
    check(rc > 0, "asprintf failed");
    check(cmd != NULL, "failed to function name");

    char *eargv[] = { "bash", "--rcfile", SHAKEFILE_NAME, "-i",
                      "-c",   cmd,        SHAKEFILE_NAME, NULL };

    int eargc = sizeof(eargv) / sizeof(char *);
    int rargc = argc - 1;

    char **xargv = malloc((eargc + rargc) * sizeof(char *));
    check_mem(xargv);

    tmp = memcpy(&xargv[0], &eargv[0], eargc * sizeof(char *));
    check_mem(tmp);
    memcpy(&xargv[eargc - 1], &argv[1], rargc * sizeof(char *));
    check_mem(tmp);

    rc = chdir(cwd);
    check(rc == 0, "chdir failed");
    return execvp("bash", xargv);

error:
    return -1;
}