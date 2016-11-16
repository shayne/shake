//
// Created by shayne on 11/15/16.
//

#include <bstrlib.h>
#include <glob.h>
#include <libgen.h>

#include "commands.h"
#include "config.h"
#include "dbg.h"
#include "shakefile/shakefile.h"

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

char *parseScriptDesc(char *filename)
{
    char *ret = NULL;
    FILE *fp = NULL;
    struct bStream *stream;
    bstring line = bfromcstr("");

    struct tagbstring key = bsStatic("desc:");

    fp = fopen(filename, "r");

    stream = bsopen((bNread)fread, fp);

    while (bsreadln(line, stream, '\n') != BSTR_ERR) {
        if (bchar(line, 0) == '#') {
            int pos = binstrcaseless(line, 0, &key);
            if (pos != BSTR_ERR) {
                bdelete(line, 0, pos + key.slen);
                btrimws(line);
                ret = bstr2cstr(line, '\0');
                break;
            }
        }
    }

    bdestroy(line);
    bsclose(stream);
    fclose(fp);

    return ret;
}

int loadCommands(char ***cmds_out, char ***descs_out)
{
    int i;
    int rc;
    int fncount = 0;
    char **cmds = NULL;
    char **descs = NULL;
    int size = 0;
    glob_t globbuf;

    char *fns[MAX_FNS];
    char *fndescs[MAX_FNS];

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
    Shakefile_detect_descriptions(MAX_FNS, fndescs);

    size = fncount + (int)globbuf.gl_pathc;
    cmds = calloc((size_t)(size + 1), sizeof(char *));
    check_mem(cmds);
    descs = calloc((size_t)(size + 1), sizeof(char *));
    check_mem(descs);

    for (i = 0; i < globbuf.gl_pathc; i++) {
        char *gl_path = globbuf.gl_pathv[i];
        char *cmd = makescriptname2(gl_path);
        check(cmd != NULL, "makescriptname2 failed");
        cmds[i] = cmd;
        descs[i] = parseScriptDesc(gl_path);
    }
    for (i = 0; i < fncount; i++) {
        size_t off = i + globbuf.gl_pathc;
        check(fns[i], "null fn");
        char *cmd = strdup(fns[i]);
        check_mem(cmd);
        cmds[off] = cmd;
        if (fndescs[i])
            descs[off] = strdup(fndescs[i]);
    }

done: // fallthrough
    *cmds_out = cmds;
    *descs_out = descs;

    globfree(&globbuf);
    free(pat);
    for (i = 0; i < fncount; i++) {
        if (fns[i])
            free(fns[i]);
        if (fndescs[i])
            free(fndescs[i]);
    }

    return size;

error:
    if (cmds) {
        for (i = 0; i < size; i++)
            free(cmds[i]);
        free(cmds);
        cmds = NULL;
    }
    if (descs) {
        for (i = 0; i < size; i++)
            if (descs[i])
                free(descs[i]);
        free(descs);
        descs = NULL;
    }
    for (i = 0; i < fncount; i++) {
        if (fns[i])
            free(fns[i]);
        if (fndescs[i])
            free(fndescs[i]);
    }
    size = -1;
    goto done;
}

void loadCommandsSorted(void)
{
}
