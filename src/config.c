//
// Created by shayne on 11/14/16.
//

#include "config.h"
#include <bstrlib.h>
#include <libgen.h>
#include <stdlib.h>
#include "dbg.h"
#include "log.h"
#include "shakefile/shakefile.h"
#include "util.h"

char const *SHAKEFILE_NAME = "Shakefile";
char const *COMMANDS_DIR = "COMMANDS_DIR";
char const *COMMANDS_PREFIX = "COMMAND_PREFIX";

char const *DEFAULT_CMD_DIR = "scripts";
char const *DEFAULT_CMD_PREFIX = "cmd-";

void initConfig(void) {
    config.proj_file = NULL;
    config.proj_dir = NULL;
    config.cmd_dir = (char *)DEFAULT_CMD_DIR;
    config.cmd_dir_len = 7;
    config.cmd_prefix = (char *)DEFAULT_CMD_PREFIX;
    config.cmd_prefix_len = 4;
}

void destroyConfig(void) {
    if (config.proj_dir_s) free(config.proj_dir_s);
    if (config.proj_file) free(config.proj_file);
    if (config.cmd_dir != DEFAULT_CMD_DIR) free(config.cmd_dir);
    if (config.cmd_prefix != DEFAULT_CMD_PREFIX) free(config.cmd_prefix);
}

void loadConfig() {
    FILE *fp;

    autofree(char) *projfile = Shakefile_find_projfile();
    if (projfile == NULL) {
        return;
    }

    fp = fopen(projfile, "r");
    if (fp == NULL) {
        LOGE("Unable to open config file '%s'\n", projfile);
        return;
    }

    config.proj_file = strdup(projfile);
    config.proj_dir = dirname(config.proj_dir_s = strdup(projfile));

    bstring line;
    while ((line = bgets((bNgetc)fgetc, fp, '\n')) != NULL) {
        btrimws(line);

        struct bstrList *args;
        args = bsplit(line, '=');

        if (biseqcstrcaseless(args->entry[0], COMMANDS_PREFIX) &&
            args->qty == 2) {
            bstring prefix;
            prefix = args->entry[1];
            if ('"' == bchar(prefix, 0) &&
                '"' == bchar(prefix, prefix->slen - 1)) {
                prefix = bmidstr(prefix, 1, prefix->slen - 2);
            }
            config.cmd_prefix = bstr2cstr(prefix, '\0');
            config.cmd_prefix_len = prefix->slen;
            if (prefix != args->entry[1]) {
                bdestroy(prefix);
            }
        } else if (biseqcstrcaseless(args->entry[0], COMMANDS_DIR) &&
                   args->qty == 2) {
            bstring dir;
            dir = args->entry[1];
            if ('"' == bchar(dir, 0) && '"' == bchar(dir, dir->slen - 1)) {
                dir = bmidstr(dir, 1, dir->slen - 2);
            }
            config.cmd_dir = bstr2cstr(dir, '\0');
            config.cmd_dir_len = dir->slen;
            if (dir != args->entry[1]) {
                bdestroy(dir);
            }
        }

        bstrListDestroy(args);
        bdestroy(line);
    }
    fclose(fp);
}
