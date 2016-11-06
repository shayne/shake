#include <stdlib.h>

#include "dbg.h"

#include "args.h"
#include "runner.h"
#include "scripts.h"

int main(int argc, char *argv[])
{
    int rc;
    Arguments_args *args = NULL;
    Scripts *scripts = NULL;

    args = Arguments_create();
    check(args != NULL, "Failed to create args");

    rc = Arguments_parse(argc, argv, args);
    check(rc == 0, "Failed to parse CLI");

    if (args->script) {
        rc = Runner_run(args->script, args->script_argv);
        // does not return on success
        check(rc == 0, "Failed to run script: %s", Script_name(args->script));
        check(0, "Check reached after run!");
        return 1;
    }

    Arguments_destroy(args);
    args = NULL;

    scripts = Scripts_init();
    Scripts_scan(scripts);

    KeyValueNode *node = NULL;
    while ((node = Scripts_nextfile(scripts)) != NULL) {
        printf("- %s\n", node->key);
        Script *script = node->value;
        printf("-- %s\n", Script_path(script));
    }

    Scripts_destroy(scripts);
    scripts = NULL;

    return 0;
error:
    if (args)
        Arguments_destroy(args);
    if (scripts)
        Scripts_destroy(scripts);
    return 1;
}
