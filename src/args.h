#ifndef _args_h
#define _args_h

#include <argp.h>

#define PROGRAM_VERSION "shake 0.1.0"
#define PROGRAM_BUG_ADDRESS "<shaynesweeney@me.com>"
#define PROGRAM_DOC "Shake replaces make"

static const struct argp_option argp_options[] = { { 0, 0, 0, 0, 0, 0 } };

typedef struct Arguments_args {
    char *script_name;
    char **script_argv;
} Arguments_args;

Arguments_args *Arguments_create();
void Arguments_destroy(Arguments_args *args);

error_t Arguments_parse(int argc, char *argv[], Arguments_args *args);

#endif
