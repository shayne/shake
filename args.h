#ifndef _args_h
#define _args_h

#include <argp.h>

#define PROGRAM_VERSION "shake 0.1.0"
#define PROGRAM_BUG_ADDRESS "<shaynesweeney@me.com>"
#define PROGRAM_DOC "Shake replaces make"

static const struct argp_option argp_options[] =
    { {
          .name = "foo", .key = 'f', .arg = "FOOARG", .flags = 0, .doc = "foodoc",
      },
      { 0 } };

typedef struct Arguments_args {
    char *args[255];
    char *f;
} Arguments_args;

Arguments_args *Arguments_create(void);
void Arguments_destroy(Arguments_args *args);

void Arguments_parse(int argc, char *argv[], Arguments_args *args);

#endif
