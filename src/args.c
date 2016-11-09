#include <stdlib.h>

#include "args.h"

const char *argp_program_version = PROGRAM_VERSION;
const char *argp_program_bug_address = PROGRAM_BUG_ADDRESS;

static error_t argp_parser(int key, char *arg, struct argp_state *state)
{
    Arguments_args *args = state->input;

    //    if (args->script) {
    //        return 0;
    //    }

    switch (key) {
    case ARGP_KEY_ARG:
        if (state->arg_num == 0) {
            args->script_name = arg;
            // set the remaining argv as the argv for script
            args->script_argv = &state->argv[state->next - 1];
            // move next to the end, stop parsing
            state->next = state->argc;
        } else {
            args->script_argv[state->arg_num] = arg;
        }
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

static struct argp argp = {.options = NULL, // argp_options,
                           .parser = argp_parser,
                           .args_doc = NULL,
                           .doc = PROGRAM_DOC,
                           .children = NULL,
                           .help_filter = NULL,
                           .argp_domain = NULL };

error_t Arguments_parse(int argc, char *argv[], Arguments_args *args)
{
    return argp_parse(&argp, argc, argv, ARGP_IN_ORDER, 0, args);
}
