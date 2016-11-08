#include <glob.h>
#include <stdlib.h>
#include <zconf.h>

#include "dbg.h"

#include "script.h"
#include "scripts.h"

Scripts *Scripts_init(char *cwd)
{
    Scripts *s;
    s = calloc(1, sizeof(Scripts));
    s->cwd = cwd;
}

void Scripts_destroy(Scripts *scripts)
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
    free(scripts);
}

char *Scripts_makename(char *path)
{
    char *pch;
    const char *prefix = "scripts/run-";
    pch = strstr(path, prefix);
    check(pch - path == 0, "Script does not start with prefix");

    char *pns = pch + strlen(prefix);

    char *pne = strrchr(pns, '.');
    long size = pne - pns;
    check(size > 0, "Size not greater than 0");

    char *name = calloc((size_t)size + 1, sizeof(char));
    strncpy(name, pns, (size_t)size);

    return name;
error:
    return NULL;
}

void Scripts_populate(Scripts *scripts, char **pathv, size_t pathc)
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

void Scripts_scan(Scripts *scripts)
{
    glob_t globbuf;

    chdir(scripts->cwd);
    glob("scripts/run-*", 0, NULL, &globbuf);

    scripts->count = globbuf.gl_pathc;

    Scripts_populate(scripts, globbuf.gl_pathv, globbuf.gl_pathc);

    globfree(&globbuf);
}

KeyValueNode *Scripts_nextfile(Scripts *scripts)
{
    // check that we didn't hit a null/empty node/file
    if (*(void **)scripts->filesiter) {
        return scripts->filesiter++;
    }
    return NULL;
}
