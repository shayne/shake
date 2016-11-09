//
// Created by shayne on 11/7/16.
//

#include "../dbg.h"
#include "shakefile.h"
#include <sys/param.h>
#include <zconf.h>
#include <stdlib.h>

#define SHAKEFILE_NAME "Shakefile"

int up_dir(char *root, char *out, size_t size)
{
    int rc = 0;
    char pathmax[size];

    rc = snprintf(pathmax, size, "%s/..", root);
    check(rc > 0, "failed to adjust path");
    check(realpath(pathmax, out) != NULL, "Failed to generate realpath");

    if (strlen(out) == 1) {
        return 1;
    }

    return 0;
error:
    return -1;
}

char *search_up(char *filename, char *startdir)
{
    char cwd[PATH_MAX];
    char nextdir[PATH_MAX];
    char file_path[PATH_MAX];

    memcpy(nextdir, startdir, PATH_MAX);

    do {
        snprintf(file_path, PATH_MAX, "%s/%s", nextdir, filename);
        if (access(file_path, F_OK) == 0) {
            return strdup(file_path);
        }
        memcpy(cwd, nextdir, PATH_MAX);
    } while (up_dir(cwd, nextdir, PATH_MAX) == 0);

    return NULL;
}

char *Shakefile_find_projfile(int *success)
{
    char *ret;
    char cwd[PATH_MAX];

    ret = getcwd(cwd, PATH_MAX);
    check(ret != NULL, "getcwd failed");

    ret = search_up(SHAKEFILE_NAME, cwd);
    check(ret != NULL, "Failed to search up");

    *success = 0;
    return ret;

error:
    *success = -1;
    return NULL;
}