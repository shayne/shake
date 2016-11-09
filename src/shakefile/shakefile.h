//
// Created by shayne on 11/7/16.
//

#ifndef SHAKE_SHAKEFILE_H
#define SHAKE_SHAKEFILE_H

#include "../utils.h"

typedef struct {
    char *file_path;
    char *root_path;
} Shakefile;

Shakefile *Shakefile_create();
void Shakefile_destroy(Shakefile *file);

char *Shakefile_find_projfile(int *success);
int Shakefile_detect_functions(char *projfile, size_t size, char *fns[]);

#endif // SHAKE_SHAKEFILE_H
