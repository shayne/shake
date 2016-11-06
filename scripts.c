#include <glob.h>
#include <stdlib.h>

#include "dbg.h"

#include "scripts.h"

Scripts *Scripts_init() { return calloc(1, sizeof(Scripts)); }

void Scripts_destroy(Scripts *scripts) {
  if (scripts->files)
    KeyValueVec_destroy(scripts->files);
  free(scripts);
}

void Scripts_populate(Scripts *scripts, char **pathv, size_t pathc) {
  if (!scripts->files) {
    scripts->files = KeyValueVec_create(pathc);
    check(scripts->files != NULL, "Files was null");
    scripts->filesiter = scripts->files->nodes;
    check(scripts->filesiter != NULL, "filesiter was null");
  }

  size_t i = 0;
  for (i = 0; i < pathc; i++, pathv++) {
    char *key = *pathv;
    KeyValueVec_set(scripts->files, key, key);
  }

error:
  return;
}

void Scripts_scan(Scripts *scripts) {
  glob_t globbuf;

  glob("scripts/run-*", 0, NULL, &globbuf);

  scripts->count = globbuf.gl_pathc;

  Scripts_populate(scripts, globbuf.gl_pathv, globbuf.gl_pathc);

  globfree(&globbuf);
}

KeyValueNode *Scripts_nextfile(Scripts *scripts) {
  // check that we didn't hit a null/empty node/file
  if (*(void **)scripts->filesiter) {
    return scripts->filesiter++;
  }
  return NULL;
}
