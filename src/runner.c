#include <unistd.h>

#include "dbg.h"

#include "runner.h"

int Runner_run(Script *script, char *argv[])
{
    return execv(Script_path(script), argv);
}
