//
// Created by shayne on 11/9/16.
//

#include "../dbg.h"
#include <libgen.h>
#include <glob.h>
#include <malloc.h>

char *makescriptname(char *path)
{
    char *out = NULL;
    char *path_cp = NULL;
    char *path_base = basename(path_cp = strdup(path));

    // FIXME: remove "cmd-" prefix
    int prefpos = 4;
    size_t cplen = strlen(path_base) - 4;
    memcpy(&path_base[0], &path_base[prefpos], cplen);
    path_base[cplen] = '\0';

    char *dot = strrchr(path_base, '.');
    if (dot && dot != path_base) {
        path_base[dot - path_base] = '\0'; // trim extension
    }

    out = strdup(path_base);
    check_mem(out);

error: // fallthrough
    free(path_cp);
    return out;
}

int Runscripts_find_script(char *cmd_name, char *cwd, char **out)
{
    int i;
    int rc;
    glob_t globbuf;

    char *pat = NULL;
    rc = asprintf(&pat, "%s/scripts/run-*", cwd); // FIXME: hardcoding
    check(rc > 0, "failed to format string");

    rc = glob(pat, 0, NULL, &globbuf);
    check(rc == 0, "glob failed");

    size_t cmd_name_l = strlen(cmd_name);

    rc = 1; // default to failure
    for (i = 0; i < globbuf.gl_pathc; i++) {
        char *gl_path = globbuf.gl_pathv[i];
        char *script_name = makescriptname(gl_path);
        check_debug(script_name != NULL, "failed to make script name");

        size_t script_name_l = strlen(script_name);
        if (script_name_l == cmd_name_l) {
            rc = strcasecmp(script_name, cmd_name);
            if (rc == 0) {
                *out = strdup(gl_path);
                free(script_name);
                check(out != NULL, "strdup fname failed");
                break;
            }
        }
        free(script_name);
    }

error: // fallthrough
    globfree(&globbuf);
    free(pat);

    return rc;
}