#include <bstrlib.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "colors.h"
#include "commands.h"
#include "config.h"
#include "runner.h"
#include "runscripts/runscripts.h"
#include "shakefile/shakefile.h"

struct shakeConfig config;

void printCommands()
{
    int i;
    int rc;
    char **cmds;
    char *fmt = NULL;
    error_t cmdcount = 0;

    cmdcount = loadCommands(&cmds);
    check(cmdcount >= 0, "loadCommands failed");

    size_t padding = 0;
    for (i = 0; i < cmdcount; i++) {
        size_t cmdlen = strlen(cmds[i]);
        if (cmdlen > padding) {
            padding = cmdlen;
        }
    }

    printf(ANSI_BOLD("Commands") "\n\n");

    for (i = 0; i < cmdcount; i++) {
        char *cmd = cmds[i];
        rc = asprintf(&fmt,
                      "  " ANSI_GREEN("%%s") "%%%lus%%s\n",
                      padding + 2 - strlen(cmd));
        check(rc > 0, "asprintf failed");
        check(fmt != NULL, "asprintf failed");
        printf(fmt, cmd, "", "description");
        free(fmt);
        fmt = NULL;
    }

    printf("\n");

error: // fallthrough
    if (cmds) {
        for (i = 0; i < cmdcount; i++) {
            free(cmds[i]);
        }
        free(cmds);
    }
    if (fmt)
        free(fmt);
}

int run_script(char *cmd_name, int argc, char *argv[])
{
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

static void usage(void)
{
    char *version = "0.0.0";
    fprintf(
        stderr,
        ANSI_BOLD("shake") " %s\n"
        "\n"
        "  shake " ANSI_PINK("[OPTIONS] ") ANSI_GREEN("[COMMAND] ") ANSI_GRAY("[COMMAND-ARGS]") "\n"
        "\n"
        ANSI_BOLD("Options") "\n"
        "  " ANSI_PINK("--init") "\tInitialize a new shake project\n"
        "  " ANSI_PINK("-h, --help") "\tPrint this help text and exit\n"
        "\n",
        version);
}

void print_no_project()
{
    printf(
        "It appears this directory, or its parent directories, "
        "have not been setup to run shake.\n");
    printf(
        "To start using shake run:\n"
        "$ shake --init\n");
}

void cleanup()
{
    destroyConfig();
}

int main(int argc, char *argv[])
{
    int rc;

    initConfig();

    char *projfile = Shakefile_find_projfile();
    if (projfile == NULL) {
        usage();
        print_no_project();
        exit(1);
    }

    loadConfig(projfile);
    atexit(cleanup); // might not be a good idea
    free(projfile);

    if (argc == 1) {
        usage();
        printCommands();
        exit(0);
    }

    if (argv[1][0] == '-') {
        fprintf(stderr, "Flag passed: %s\n", argv[1]);
        usage();
        exit(1);
    }

    char *script_name = argv[1];
    rc = run_script(script_name, argc, &argv[1]);
    if (rc != 0) {
        fprintf(stderr, "Unknown script/fn: '%s'\n", script_name);
        exit(1);
    }

    return 0;
}
