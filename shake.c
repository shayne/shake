#include <stdlib.h>
#include "dbg.h"

#include "args.h"
#include "scripts.h"


int main(int argc, char *argv[])
{
    Arguments_args *args = Arguments_create();
    Arguments_parse(argc, argv, args);
    
    if (args->f) {
        debug("F was set: %s", args->f);
    }

    Arguments_destroy(args);

    Scripts * scripts = Scripts_init();
    Scripts_scan(scripts);

    debug("Scripts count: %d", scripts->count);

    KeyValueNode * node = NULL;
    while ((node = Scripts_nextfile(scripts)) != NULL) {
        debug("SCRIPT: %s", node->value);
    }

    Scripts_destroy(scripts);

    return 0;
}
