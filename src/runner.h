#ifndef _runner_h
#define _runner_h

#include "script.h"

int Runner_run(char *cwd, char *script_name, char **argv);
int Runner_runfn(char *fn, char *cwd, int argc, char *argv[]);

#endif
