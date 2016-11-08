#ifndef _scripts_h
#define _scripts_h

#include "keyvaluevec.h"

struct Scripts {
    size_t count;
    char *cwd;
    KeyValueVec *files;
    KeyValueNode *filesiter;
};

void Scripts_destroy(struct Scripts *scripts);

void Scripts_scan(struct Scripts *scripts);

KeyValueNode *Scripts_nextfile(struct Scripts *scripts);

#endif
