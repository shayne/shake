#include <stdlib.h>

#include "args.h"
#include "dbg.h"

Arguments_args *Arguments_create() {
  argp_program_version = PROGRAM_VERSION;
  argp_program_bug_address = PROGRAM_BUG_ADDRESS;

  Arguments_args *args = NULL;
  args = calloc(1, sizeof(Arguments_args));
  check_mem(args);

error: // fallthrough
  return args;
}

void Arguments_destroy(Arguments_args *args) { free(args); }

static error_t argp_parser(int key, char *arg, struct argp_state *state) {
  Arguments_args *args = state->input;

  switch (key) {
  case 'f':
    args->f = arg;
    break;

  case ARGP_KEY_ARG:
    /* if (state->arg_num >= 1) */
    /* argp_usage(state); */
    args->args[state->arg_num] = arg;
    break;

  case ARGP_KEY_END:
    /* if (state->arg_num < 2) */
    /* argp_usage(state); */
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {.options = argp_options,
                           .parser = argp_parser,
                           .args_doc = NULL,
                           .doc = PROGRAM_DOC,
                           .children = NULL,
                           .help_filter = NULL,
                           .argp_domain = NULL};

void Arguments_parse(int argc, char *argv[], Arguments_args *args) {
  argp_parse(&argp, argc, argv, 0, 0, args);
}
