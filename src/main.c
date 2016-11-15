#include <bstrlib.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "config.h"
#include "runner.h"
#include "runscripts/runscripts.h"
#include "shakefile/shakefile.h"

struct shakeConfig config;

void print_fns()
{
    Runscripts_print_scripts();
    Shakefile_print_fns();
}

int run_script(char *cmd_name, int argc, char *argv[])
{
    int rc;
    char *runscript = NULL;

    char *projfile = Shakefile_find_projfile();
    char *cwd = dirname(strdup(projfile));

    rc = Runscripts_find_script(cmd_name, &runscript);
    if (rc == 0) {
        check(runscript != NULL, "runscript was null");
        rc = Runner_run(runscript, cwd, argv);
    }

    if (Shakefile_has_fn(cmd_name)) {
        rc = Runner_runfn(cmd_name, cwd, argc, argv);
    }

error:
    free(runscript);
    free(projfile);
    free(cwd);

    return rc;
}

void print_usage()
{
    printf("TODO: usage\n");
}

void print_no_project()
{
    printf(
        "It appears this directory, or its parent directories, "
        "have not been setup to run shake.\n");
    printf(
        "To start using shake run:\n"
        "$ shake --init\n");
}

void cleanup()
{
    destroyConfig();
}

int main(int argc, char *argv[])
{
    int rc;

    initConfig();

    char *projfile = Shakefile_find_projfile();
    if (projfile == NULL) {
        print_usage();
        print_no_project();
        exit(1);
    }

    loadConfig(projfile);
    atexit(cleanup); // might not be a good idea
    free(projfile);

    if (argc == 1) {
        print_fns();
        exit(0);
    }

    if (argv[1][0] == '-') {
        fprintf(stderr, "Flag passed: %s\n", argv[1]);
        exit(1);
    }

    char *script_name = argv[1];
    rc = run_script(script_name, argc, &argv[1]);
    if (rc != 0) {
        fprintf(stderr, "Unknown script/fn: '%s'\n", script_name);
        exit(1);
    }

    return 0;
}
