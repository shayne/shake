#include <stdlib.h>

#include "dbg.h"

#include "args.h"
#include "runner.h"
#include "script.h"
#include "scripts.h"

int main(int argc, char *argv[])
{
    Arguments_args *args = Arguments_create();
    Arguments_parse(argc, argv, args);

    if (args->f) {
        debug("F was set: %s", args->f);
    }

    if (args->args[0] != NULL) {
        Runner_run(args->args[0], &args->args[0]);
    }

    Arguments_destroy(args);
    args = NULL;

    Scripts *scripts = Scripts_init();
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
}
