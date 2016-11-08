#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#include "dbg.h"

#include "args.h"
#include "runner.h"
#include "scripts.h"
#include "shakefile/shakefile.h"

#define TCOLOR(C, S)  C S TC_CLEAR
#define TC_CLEAR      "\x1b[0m"
#define TC_RED(S)     TCOLOR("\x1b[31m", S)
#define TC_GREEN(S)   TCOLOR("\x1b[32m", S)
#define TC_YELLOW(S)  TCOLOR("\x1b[33m", S)
#define TC_BLUE(S)    TCOLOR("\x1b[34m", S)
#define TC_MAGENTA(S) TCOLOR("\x1b[35m", S)
#define TC_CYAN(S)    TCOLOR("\x1b[36m", S)

int main(int argc, char *argv[])
{
    int rc;
    Arguments_args *args = NULL;
    Scripts *scripts = NULL;

    char *projfile = Shakefile_find_projfile(&rc);
    char *cwd = dirname(projfile);
    check(rc == 0, "Failed to detect project root");

//    /** SHAKE FILE TESTS **/
//    if (argc > 1 && strcmp(argv[1], "foo") == 0) {
//        FILE *fp;
//        fp = popen("bash --rcfile Shakefile -i -c 'compgen -A function'", "r");
//        check(fp != NULL, "failed to open pipe");
//
//        char buf[255];
//        while(fgets(buf, 255, fp) != NULL) {
//            printf("buf: %s", buf);
//        }
//
//        pclose(fp);
//
//        debug("arg: %s", argv[2]);
//        debug("arg: %s", argv[3]);
//        debug("arg: %s", argv[4]);
//        char *eargv[255] = { "bash", "--rcfile", "Shakefile", "-i", "-c", "cmd-foo", "--", argv[2], argv[3], argv[4], NULL };
//        rc = execvp("bash", eargv);
//        check(rc == 0, "failed to execvp");
//    }
//    /** **/

    args = Arguments_create(projfile);
    check(args != NULL, "Failed to create args");

    rc = Arguments_parse(argc, argv, args);
    check(rc == 0, "Failed to parse CLI");

    if (args->script) {
        rc = Runner_run(cwd, args->script, args->script_argv);
        // does not return on success
        check(rc == 0, "Failed to run script: %s", Script_name(args->script));
        check(0, "Check reached after run!");
        return 1;
    }

    Arguments_destroy(args);

    scripts = Scripts_init(cwd);
    Scripts_scan(scripts);

    KeyValueNode *node = NULL;
    while ((node = Scripts_nextfile(scripts)) != NULL) {
        printf(TC_RED("-") " " TC_GREEN("%s") "\n", node->key);
        Script *script = node->value;
        printf("-- " TC_MAGENTA("%s") "\n", Script_path(script));
    }

    Scripts_destroy(scripts);

    return 0;
error:
    if (projfile)
        free(projfile);
    if (args)
        Arguments_destroy(args);
    if (scripts)
        Scripts_destroy(scripts);
    return 1;
}

