//
// Created by shayne on 11/7/16.
//

#ifndef SHAKE_SHAKEFILE_H
#define SHAKE_SHAKEFILE_H

#include <stdlib.h>

char *Shakefile_find_projfile(void);
int Shakefile_detect_functions(size_t size, char *fns[]);
int Shakefile_has_fn(char *name);

#endif // SHAKE_SHAKEFILE_H
