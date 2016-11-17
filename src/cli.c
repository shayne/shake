//
// Created by shayne on 11/16/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <ctype.h>
#include <bstrlib.h>
#include <sys/stat.h>
#include <limits.h>
#include "cli.h"
#include "colors.h"
#include "dbg.h"
#include "config.h"

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

char *readWithPreset(char *prompt, char *preset)
{
    char ch;
    char *out = NULL;
    bstring value = cstr2bstr(preset);
    printf("%s%s", prompt, preset);
    fflush(stdout);
    while ((ch = getch()) != 0) {
        switch (ch) {
            case '\177': // backspace
                if (value->slen > 0) {
                    bdelete(value, value->slen - 1, 1);
                    printf("\b \b");
                }
                break;
            case '\n': // enter
                printf("\n");
                goto end;
            default:
                if (isprint(ch)) {
                    bconchar(value, ch);
                    printf("%c", ch);
                }
        }
        fflush(stdout);
    }
    end:
    out = bstr2cstr(value, '\0');
    bdestroy(value);
    return out;
}


void cliInitGetConfig(void)
{
    int rc;
    char *projectDir;
    char *commandDir;
    char *commandPrefix;

    // projectDir
    printf(ANSI_BOLD("#1) Choose a project root\n"));
    printf("    This directory will the top-most directory in your project\n");
    printf("    that will recognize your shake commands.\n");
    printf("\n");
    printf(ANSI_GREEN("TIP:") " You can run shake from any sub-directory of your project");
    printf("\n");

    char cwd[PATH_MAX];
    check(getcwd(cwd, PATH_MAX) != NULL, "getcwd failed");

    while (1) {
        projectDir = readWithPreset(ANSI_BOLD("root dir: "), cwd);
        struct stat projectDirStat;
        if (access(projectDir, W_OK)) {
            fprintf(stderr, COLOR_ERROR("[ERROR]") " Project root '%s' does not have write permission\n", projectDir);
            continue;
        }
        rc = stat(projectDir, &projectDirStat);
        if (rc == 0 && S_ISDIR(projectDirStat.st_mode) != 1) {
            fprintf(stderr, COLOR_ERROR("[ERROR]") " Project root '%s' is not a directory\n", projectDir);
            continue;
        }
        break;
    }

    // cmd_dir
    printf("\n");
    printf(ANSI_BOLD("#2) Specify a command-scripts directory\n"));
    printf("    This directory can be a new or existing one.\n");
    printf("    It will be used by shake to locate command-scripts.\n");
    printf("\n");
    printf(ANSI_GREEN("TIP:") " Command-scripts are normal scripts that can be executed directly.\n");
    printf("     It's common to choose a directory you already have and have command\n");
    printf("     scripts wrap existing sets of scripts or use common helpers.\n");
    printf("\n");

    char *prompt;
    asprintf(&prompt, ANSI_BOLD("commands dir:") " %s/", projectDir);

    while (1) {
        commandDir = readWithPreset(prompt, DEFAULT_CMD_DIR);
        if (strcmp(commandDir, "/") == 0 || strcmp(commandDir, "..") == 0) {
            fprintf(stderr, COLOR_ERROR("[ERROR]") " Must provide the name of a sub-directory\n");
            continue;
        }
        printf("\n");
        break;
    }

    free(prompt);

    // cmd_prefix
    printf(ANSI_BOLD("#3) Specify a command-script prefix\n"));
    printf("    This prefix is used by shake to separate command-scripts from other files in the\n");
    printf("    command-scripts directory. The prefix is not part of the name of the command.\n");
    printf("    It's recommended to end the prefix with a \"-\", for example:\n");
    printf("      $ shake start-server # command-script: cmd-start-server\n");
    printf("      $ shake run-tests    # command-script: cmd-run-tests\n");
    printf("\n");
    printf(ANSI_GREEN("TIP:") " You can edit existing or create new command-scripts by running:\n");
    printf("       $ shake --edit my-existing-command\n");
    printf("       $ shake --create some-new-command\n");
    printf("\n");

    while (1) {
        commandPrefix = readWithPreset(ANSI_BOLD("command prefix: "), DEFAULT_CMD_PREFIX);
        if (strlen(commandPrefix) == 0) {
            fprintf(stderr, COLOR_ERROR("[ERROR]") " Must specify a prefix for commands\n");
            continue;
        }
        printf("\n");
        break;
    }

    // confirm
    printf("\n");
    printf("Confirm configuration:\n");
    printf("\n");
    printf("#1) "ANSI_BOLD("root dir      ") " = %s\n", projectDir);
    printf("#2) "ANSI_BOLD("commands dir  ") " = %s\n", commandDir);
    printf("#3) "ANSI_BOLD("command prefix") " = %s\n", commandPrefix);
    printf("\n");

    int restart = 0;
    char *yesOrNo = readWithPreset(ANSI_BOLD("Does this look right? "), "y");
    if (strlen(yesOrNo) > 0 && tolower(yesOrNo[0]) == 'n') {
        restart = 1;
    } else {
        config.proj_dir_s = config.proj_dir = strdup(projectDir); // FIXME: lame
        config.cmd_dir = strdup(commandDir);
        config.cmd_prefix = strdup(commandPrefix);
    }
    free(yesOrNo);

    if (projectDir)
        free(projectDir);
    if (commandDir)
        free(commandDir);
    if (commandPrefix)
        free(commandPrefix);

    if (restart) {
        cliInitGetConfig();
    }

    return;

error:
    if (projectDir)
        free(projectDir);
    if (commandDir)
        free(commandDir);
    if (commandPrefix)
        free(commandPrefix);
    fprintf(stderr, COLOR_ERROR("[ERROR]") " Failed init");
}

void cliInit(void)
{
    printf("\n");
    printf(ANSI_GRAY("This utility will walk you through the basic shake setup process.\n"));
    printf(ANSI_GRAY("It only covers the required configuration, and suggests typical defaults.\n"));
    printf("\n");
    printf(ANSI_GRAY("Press CTRL-C at any time to quit. No changes are made until you've finished.\n"));
    printf("\n");

    cliInitGetConfig();

    exit(0);
}