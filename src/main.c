#include <bstrlib.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "runner.h"
#include "runscripts/runscripts.h"
#include "scripts.h"
#include "shakefile/shakefile.h"

#define TCOLOR(C, S) C S TC_CLEAR
#define TC_CLEAR "\x1b[0m"
#define TC_RED(S) TCOLOR("\x1b[31m", S)
#define TC_GREEN(S) TCOLOR("\x1b[32m", S)
#define TC_YELLOW(S) TCOLOR("\x1b[33m", S)
#define TC_BLUE(S) TCOLOR("\x1b[34m", S)
#define TC_MAGENTA(S) TCOLOR("\x1b[35m", S)
#define TC_CYAN(S) TCOLOR("\x1b[36m", S)

void print_fns()
{
    int i;
    int rc;
    int fncount;
    char *fns[255];
    struct Scripts scripts = { 0 };

    char *projfile = Shakefile_find_projfile(&rc);
    char *cwd = dirname(strdup(projfile));

    scripts.cwd = cwd;
    Scripts_scan(&scripts);

    check(rc == 0, "Failed to detect project root");
    fncount = Shakefile_detect_functions(projfile, 255, fns);

    KeyValueNode *node = NULL;
    while ((node = Scripts_nextfile(&scripts)) != NULL) {
        printf(TC_RED("-") " " TC_GREEN("%s") "\n", node->key);
        Script *script = node->value;
        printf("-- " TC_MAGENTA("%s") "\n", Script_path(script));
    }

    for (i = 0; i < fncount; i++) {
        printf(TC_RED("-") " " TC_GREEN("%s") "\n", fns[i]);
        printf("-- " TC_MAGENTA("Shakefile") "\n");
    }

    free(projfile);
    free(cwd);
    Scripts_destroy(&scripts);
    for (i = 0; i < fncount; i++)
        free(fns[i]);

error:
    return;
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
