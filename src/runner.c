#include <unistd.h>

#include "dbg.h"

#include "runner.h"

extern char **environ;

int Runner_run(Script *script, char *argv[])
{
    return execve(Script_path(script), argv, environ);
}
