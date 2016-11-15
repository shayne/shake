#include <bstrlib.h>
#include <glob.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#include "colors.h"
#include "config.h"
#include "runner.h"
#include "runscripts/runscripts.h"
#include "shakefile/shakefile.h"

struct shakeConfig config;

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

void print_fns() // TODO: major cleanup, sorting commands
{
    int i;
    int rc;
    int fncount;
    glob_t globbuf;
    char *fns[255];

    char *pat = NULL;
    rc = asprintf(&pat,
                  "%s/%s/%s*",
                  config.proj_dir,
                  config.cmd_dir,
                  config.cmd_prefix);
    check(rc > 0, "asprintf failed");
    check_mem(pat);

    rc = glob(pat, 0, NULL, &globbuf);
    check_debug(rc == 0, "No scripts.");

    fncount = Shakefile_detect_functions(255, fns);

    size_t totcount = fncount + globbuf.gl_pathc;

    char **allcmds = malloc(totcount * sizeof(char *));
    check_mem(allcmds);

    size_t padding = 0;

    for (i = 0; i < globbuf.gl_pathc; i++) {
        char *gl_path = globbuf.gl_pathv[i];
        char *script_name = makescriptname2(gl_path);
        allcmds[i] = script_name; // TODO: free sometime
        size_t strlen1 = strlen(allcmds[i]);
        if (strlen1 > padding) {
            padding = strlen1;
        }
    }
    for (i = 0; i < fncount; i++) {
        size_t offset = i + globbuf.gl_pathc;
        allcmds[offset] = fns[i];
        size_t strlen1 = strlen(allcmds[offset]);
        if (strlen1 > padding) {
            padding = strlen1;
        }
    }

    printf(ANSI_BOLD("Commands") "\n\n");

    char fmt[255] = { 0 };

    for (i = 0; i < totcount; i++) {
        char *cmd = allcmds[i];
        snprintf(fmt,
                 255,
                 "  " ANSI_GREEN("%%s") "%%%lus%%s\n",
                 padding + 2 - strlen(cmd));
        check(fmt != NULL, "snprintf failed");
        printf(fmt, cmd, "", "description");
    }

    printf("\n");

    for (i = 0; i < globbuf.gl_pathc; i++)
        free(allcmds[i]);
    free(allcmds);

    for (i = 0; i < fncount; i++)
        free(fns[i]);

error: // fallthrough
    globfree(&globbuf);
    free(pat);
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
        print_fns();
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
