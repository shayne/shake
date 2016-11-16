//
// Created by shayne on 11/15/16.
//

#include "commands.h"
#include "config.h"
#include "dbg.h"
#include "shakefile/shakefile.h"
#include <glob.h>

#define MAX_FNS 255

char *makescriptname2(char *path)
{
    char *out = NULL;
    char *path_cp = NULL;
    char *path_base = basename(path_cp = strdup(path));

    int prefpos = config.cmd_prefix_len;
    size_t cplen = strlen(path_base) - prefpos;
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

int loadCommands(char ***out)
{
    int i;
    int rc;
    int fncount = 0;
    char **cmds = NULL;
    error_t size = 0;
    glob_t globbuf;

    char *fns[MAX_FNS];

    char *pat = NULL;
    rc = asprintf(&pat,
                  "%s/%s/%s*",
                  config.proj_dir,
                  config.cmd_dir,
                  config.cmd_prefix);
    check(rc > 0, "asprintf failed");
    check_mem(pat);

    rc = glob(pat, 0, NULL, &globbuf);
    check_debug(rc == 0, "glob failed");

    fncount = Shakefile_detect_functions(MAX_FNS, fns);

    size = fncount + (int)globbuf.gl_pathc;
    cmds = calloc(100, sizeof(char *));
    check_mem(cmds);

    for (i = 0; i < globbuf.gl_pathc; i++) {
        char *gl_path = globbuf.gl_pathv[i];
        char *cmd = makescriptname2(gl_path);
        check(cmd != NULL, "makescriptname2 failed");
        cmds[i] = cmd;
    }
    for (i = 0; i < fncount; i++) {
        size_t off = i + globbuf.gl_pathc;
        check(fns[i], "null fn");
        char *cmd = strdup(fns[i]);
        check_mem(cmd);
        cmds[off] = cmd;
    }

done: // fallthrough
    *out = cmds;

    globfree(&globbuf);
    free(pat);
    for (i = 0; i < fncount; i++)
        if (fns[i])
            free(fns[i]);

    return size;

error:
    size = -1;
    if (cmds) {
        free(cmds);
        cmds = NULL;
    }
    goto done;
}

void loadCommandsSorted(void)
{
}
