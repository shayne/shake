#include <unistd.h>

#include "dbg.h"

#include "runner.h"

int Runner_run(char *cwd, Script *script, char **argv)
{
    chdir(cwd);
    return execv(Script_path(script), argv);
}
