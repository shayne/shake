//
// Created by shayne on 11/7/16.
//

#ifndef SHAKE_SHAKEFILE_H
#define SHAKE_SHAKEFILE_H

#include <stdlib.h>

typedef struct Shakefile {
    char *cmd_dir;
    int cmd_dir_len;
    char *cmd_prefix;
    int cmd_prefix_len;
    char *projfile;
    char *projdir;
} Shakefile;

extern struct Shakefile gShakefile;

void Shakefile_init(void);
void Shakefile_destroy(void);

char *Shakefile_find_projfile(int *success);
int Shakefile_detect_functions(size_t size, char *fns[]);
int Shakefile_has_fn(char *name);
void Shakefile_print_fns();

#endif // SHAKE_SHAKEFILE_H
