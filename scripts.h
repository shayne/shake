#ifndef _scripts_h
#define _scripts_h

#include "keyvaluevec.h"

typedef struct Scripts {
  size_t count;
  KeyValueVec *files;
  KeyValueNode *filesiter;
} Scripts;

Scripts *Scripts_init(void);
void Scripts_destroy(Scripts *scripts);

void Scripts_scan(Scripts *scripts);

KeyValueNode *Scripts_nextfile(Scripts *scripts);

#endif
