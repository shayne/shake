//
// Created by shayne on 11/7/16.
//

#ifndef SHAKE_SHAKEFILE_H
#define SHAKE_SHAKEFILE_H

typedef struct {
    char **fns;
    char *file_path;
    char *root_path;
} Shakefile;

Shakefile *Shakefile_create();
void Shakefile_destroy(Shakefile *file);

void Shakefile_detect_functions(Shakefile *file);
char *Shakefile_find_projfile(int *success);

#endif // SHAKE_SHAKEFILE_H
