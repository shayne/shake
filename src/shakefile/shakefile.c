//
// Created by shayne on 11/7/16.
//

#include "shakefile.h"
#include "../dbg.h"
#include <stdlib.h>

Shakefile *Shakefile_create()
{
    Shakefile *file = calloc(1, sizeof(Shakefile));
    check_mem(file);
    return file;
error:
    return NULL;
}

void Shakefile_destroy(Shakefile *file)
{
    if (file->root_path)
        free(file->root_path);
    if (file->file_path)
        free(file->file_path);
    if (file)
        free(file);
}