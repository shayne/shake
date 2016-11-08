#include <bstrlib.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "args.h"
#include "runner.h"
#include "scripts.h"
#include "shakefile/shakefile.h"
#include "utils.h"

#define TCOLOR(C, S) C S TC_CLEAR
#define TC_CLEAR "\x1b[0m"
#define TC_RED(S) TCOLOR("\x1b[31m", S)
#define TC_GREEN(S) TCOLOR("\x1b[32m", S)
#define TC_YELLOW(S) TCOLOR("\x1b[33m", S)
#define TC_BLUE(S) TCOLOR("\x1b[34m", S)
#define TC_MAGENTA(S) TCOLOR("\x1b[35m", S)
#define TC_CYAN(S) TCOLOR("\x1b[36m", S)

int main(int argc, char *argv[])
{
    int i;
    int rc;
    Arguments_args *args = NULL;
    struct Scripts scripts = { 0 };

    char *projfile = Shakefile_find_projfile(&rc);
    char *cwd = dirname(strdup(projfile));
    check(rc == 0, "Failed to detect project root");

    scripts.cwd = cwd;

    CharPArray cap;
    capinit(&cap, 10);

    Shakefile_detect_functions(projfile, &cap);

    args = Arguments_create();
    check(args != NULL, "Failed to create args");

    rc = Arguments_parse(argc, argv, args);
    check(rc == 0, "Failed to parse CLI");

    if (args->script_name) {
        char *fn = NULL;
        for (i = 0; i < cap.used; i++) {
            if (strcmp(cap.array[i], args->script_name) == 0) {
                fn = cap.array[i];
                break;
            }
        }

        if (fn != NULL) {
            Runner_runfn(fn, cwd, argc, argv);
            perror("runfn");
        } else {
            Runner_run(cwd, args->script_name, args->script_argv);
            perror("run");
        }
    }

    Arguments_destroy(args);

    Scripts_scan(&scripts);

    KeyValueNode *node = NULL;
    while ((node = Scripts_nextfile(&scripts)) != NULL) {
        printf(TC_RED("-") " " TC_GREEN("%s") "\n", node->key);
        Script *script = node->value;
        printf("-- " TC_MAGENTA("%s") "\n", Script_path(script));
    }

    for (i = 0; i < cap.used; i++) {
        printf(TC_RED("-") " " TC_GREEN("%s") "\n", cap.array[i]);
        printf("-- " TC_MAGENTA("Shakefile") "\n");
    }

    capfree(&cap);
    Scripts_destroy(&scripts);
    free(projfile);
    free(cwd);

    return 0;

error:
    capfree(&cap);
    if (projfile)
        free(projfile);
    if (args)
        Arguments_destroy(args);
    Scripts_destroy(&scripts);
    return 1;
}
