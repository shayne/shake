//
// Created by shayne on 11/7/16.
//

#ifndef SHAKE_SHAKEFILE_H
#define SHAKE_SHAKEFILE_H

char *Shakefile_find_projfile(int *success);
int Shakefile_detect_functions(char *projfile, size_t size, char *fns[]);
int Shakefile_has_fn(char *name, char *projfile);

#endif // SHAKE_SHAKEFILE_H
