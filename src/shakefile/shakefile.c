//
// Created by shayne on 11/11/16.
//

#include <libgen.h>
#include "shakefile.h"
#include "../dbg.h"

Shakefile gShakefile = { 0 };

void Shakefile_init()
{
    int rc;
    char *projfile = Shakefile_find_projfile(&rc);
    if (rc != 0) {
        debug("Find projfile rc: %d", rc);
    } else {
        if (projfile == NULL) {
            debug("Projfile is null");
        } else {
            char *dir = dirname(strdup(projfile));
            if (dir == NULL) {
                debug("dirname of projfile null");
            }
            gShakefile.projdir = dir;
            gShakefile.projfile = projfile;
        }
    }

    gShakefile.cmd_dir = "scripts";
    gShakefile.cmd_dir_len = 7;
    gShakefile.cmd_prefix = "cmd-";
    gShakefile.cmd_prefix_len = 4;
}

void Shakefile_destroy()
{
    if (gShakefile.projfile)
        free(gShakefile.projfile);
    if (gShakefile.projdir)
        free(gShakefile.projdir);
}
