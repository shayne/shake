#include <bstrlib.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "runner.h"
#include "runscripts/runscripts.h"
#include "shakefile/shakefile.h"

void print_fns()
{
    int rc;
    char *projfile = NULL;
    char *cwd = NULL;

    projfile = Shakefile_find_projfile(&rc);
    check(rc == 0, "Failed to detect project root");
    cwd = dirname(strdup(projfile));

    Runscripts_print_scripts(cwd);
    Shakefile_print_fns(projfile);

error:
    if (cwd)
        free(cwd);
    if (projfile)
        free(projfile);
}

int run_script(char *cmd_name, int argc, char *argv[])
{
    int rc;
    char *runscript = NULL;

    char *projfile = Shakefile_find_projfile(&rc);
    char *cwd = dirname(strdup(projfile));
    check(rc == 0, "Failed to detect project root");

    rc = Runscripts_find_script(cmd_name, cwd, &runscript);
    if (rc == 0) {
        check(runscript != NULL, "runscript was null");
        rc = Runner_run(runscript, cwd, argv);
    }

    if (Shakefile_has_fn(cmd_name, projfile)) {
        rc = Runner_runfn(cmd_name, cwd, argc, argv);
    }

error:
    free(runscript);
    free(projfile);
    free(cwd);

    return rc;
}

int main(int argc, char *argv[])
{
    int rc;

    if (argc == 1) {
        print_fns();
        return 0;
    }

    if (argv[1][0] == '-') {
        fprintf(stderr, "Flag passed: %s", argv[1]);
        return 1;
    }

    char *script_name = argv[1];

    rc = run_script(script_name, argc, &argv[1]);
    if (rc != 0) {
        fprintf(stderr, "Unknown script/fn: '%s'\n", script_name);
    }

    return 0;
}
