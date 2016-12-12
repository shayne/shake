#include <bstrlib.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "cli.h"
#include "colors.h"
#include "commands.h"
#include "config.h"
#include "log.h"
#include "runner.h"
#include "runscripts/runscripts.h"
#include "shakefile/shakefile.h"

struct shakeConfig config;

static void printUsage(void) {
    char *version = "0.0.0";
    fprintf(stderr,
            "\n"
                    ANSI_BOLD("shake") " %s\n"
                    "\n"
                    "  shake " ANSI_PINK("[OPTIONS] ") ANSI_GREEN("[COMMAND] ") ANSI_GRAY("[COMMAND-ARGS]") "\n"
                    "\n"
                    ANSI_BOLD("Options") "\n"
                    "  " ANSI_PINK("--init") "            Initialize a new shake project\n"
                    "  " ANSI_PINK("-h, --help") "        Print this help text and exit\n"
                    "  " ANSI_PINK("--edit ") ANSI_GREEN("[COMMAND]") "  Edit the Shakefile or command script\n"
                    "\n",
            version);
}

static void printCommands(void) {
    int i;
    int rc;
    char **cmds;
    char **descs;
    char *fmt = NULL;
    ssize_t cmdcount = 0;

    printUsage();

    cmdcount = loadCommands(&cmds, &descs);
    check(cmdcount >= 0, "loadCommands failed");

    size_t padding = 0;
    for (i = 0; i < cmdcount; i++) {
        size_t cmdlen = strlen(cmds[i]);
        if (cmdlen > padding) {
            padding = cmdlen;
        }
    }

    if (cmdcount > 0) {
        printf(ANSI_BOLD("Commands") "\n\n");
    }

    for (i = 0; i < cmdcount; i++) {
        char *cmd = cmds[i];
        char *desc = descs[i];
        rc = asprintf(&fmt, "  " ANSI_GREEN("%%s") "%%%lus%%s\n",
                      padding + 2 - strlen(cmd));
        check(rc > 0, "asprintf failed");
        check(fmt != NULL, "asprintf failed");
        printf(fmt, cmd, "", desc ? desc : "");
        free(fmt);
        fmt = NULL;
    }

    printf("\n");

error:  // fallthrough
    if (cmds) {
        for (i = 0; i < cmdcount; i++) free(cmds[i]);
        free(cmds);
    }
    if (descs) {
        for (i = 0; i < cmdcount; i++)
            if (descs[i]) free(descs[i]);
        free(descs);
    }
    if (fmt) free(fmt);
}

static int run_script(char *cmd_name, int argc, char *argv[]) {
    int rc;
    char *runscript = NULL;

    char *projfile = Shakefile_find_projfile();
    char *cwd = dirname(strdup(projfile));

    rc = Runscripts_find_script(cmd_name, &runscript);
    if (rc == 0) {
        check(runscript != NULL, "runscript was null");
        rc = Runner_run(runscript, cwd, argv);
    }

    if (Shakefile_has_fn(cmd_name)) {
        rc = Runner_runfn(cmd_name, cwd, argc, argv);
    }

error:
    free(runscript);
    free(projfile);
    free(cwd);

    return rc;
}

static void usage(int rc) {
    printUsage();
    exit(rc);
}

static void noproject(void) {
    fprintf(stderr,
            "It appears this directory, or its parent directories, "
            "have not been setup to run shake.\n");
    fprintf(stderr,
            "To start using shake run:\n"
            "  $ shake --init\n\n");
    usage(1);
}

static int parseOptions(int argc, char **argv) {
    int rc;
    int i;
    int commandpos = -1;

    for (i = 1; i < argc; i++) {
        int lastarg = i == argc - 1;

        if (!strcmp(argv[i], "-h")) {
            usage(0);
        } else if (!strcmp(argv[i], "--help")) {
            usage(0);
        } else if (!strcmp(argv[i], "--init")) {
            rc = cliInit();
            if (rc != 0) {
                exit(1);
            }
            printCommands();
            exit(0);
        } else if (!strcmp(argv[i], "--edit") && !lastarg) {
            exit(cliEdit(argv[++i]));
        } else if (argv[i][0] != '-') {
            commandpos = i;
        } else if (argv[i][0] == '-' && i < commandpos) {
            LOGE("Unknown flag/option: '%s'\n", argv[i]);
            exit(1);
        }
    }
    return 0;
}

static void cleanup(void) { destroyConfig(); }

int main(int argc, char *argv[]) {
    int rc;

    initConfig();
    atexit(cleanup);  // might not be a good idea
    loadConfig();

    parseOptions(argc, argv);

    if (config.proj_file == NULL) {
        noproject();
    }

    if (argc == 1) {
        printCommands();
        exit(0);
    }

    char *script_name = argv[1];
    rc = run_script(script_name, argc, &argv[1]);
    if (rc != 0) {
        LOGE("Unknown script/fn: '%s'\n", script_name);
        exit(1);
    }

    return 0;
}
