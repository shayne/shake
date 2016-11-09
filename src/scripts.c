#include <glob.h>
#include <stdlib.h>
#include <zconf.h>

#include "dbg.h"

#include "script.h"
#include "scripts.h"

void Scripts_destroy(struct Scripts *scripts)
{
    size_t i = 0;
    if (scripts->files) {
        for (i = 0; i < scripts->count; i++) {
            KeyValueNode node = scripts->files->nodes[i];
            if (node.value) {
                Script_destroy(node.value);
            }
        }
        KeyValueVec_destroy(scripts->files);
    }
}

void Scripts_populate(struct Scripts *scripts, char **pathv, size_t pathc)
{
    if (!scripts->files) {
        scripts->files = KeyValueVec_create(pathc);
        check(scripts->files != NULL, "Files was null");

        scripts->filesiter = scripts->files->nodes;
        check(scripts->filesiter != NULL, "filesiter was null");
    }

    size_t i = 0;
    for (i = 0; i < pathc; i++, pathv++) {
        Script *script = Script_create(*pathv);
        KeyValueVec_set(scripts->files, Script_name(script), script);
    }

error:
    return;
}

void Scripts_scan(struct Scripts *scripts)
{
    glob_t globbuf;

    chdir(scripts->cwd);
    glob("scripts/run-*", 0, NULL, &globbuf);

    scripts->count = globbuf.gl_pathc;

    Scripts_populate(scripts, globbuf.gl_pathv, globbuf.gl_pathc);

    globfree(&globbuf);
}

KeyValueNode *Scripts_nextfile(struct Scripts *scripts)
{
    // check that we didn't hit a null/empty node/file
    if (*(void **)scripts->filesiter) {
        return scripts->filesiter++;
    }
    return NULL;
}
