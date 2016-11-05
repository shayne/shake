#ifndef _scripts_h
#define _scripts_h

#include "keyvaluevec.h"

typedef struct Scripts {
    int count;
    KeyValueVec * files;
    KeyValueNode * filesiter;
} Scripts;

Scripts * Scripts_init();
void Scripts_destroy(Scripts * scripts);

void Scripts_scan(Scripts * scripts);

KeyValueNode * Scripts_nextfile(Scripts * scripts);

#endif
