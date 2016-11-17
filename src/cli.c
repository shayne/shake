//
// Created by shayne on 11/16/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <bstrlib.h>
#include <sys/stat.h>
#include <limits.h> // CLion says not needed?
#include "cli.h"
#include "colors.h"
#include "dbg.h"
#include "config.h"
#include "log.h"
#include "util.h"

static char getch() {
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

static char *readWithPreset(char *prompt, char *preset)
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


static void cliInitGetConfig(void)
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

    // commandDir
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

    // commandPrefix
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
        asprintf(&config.proj_file, "%s/%s", config.proj_dir, SHAKEFILE_NAME);
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

static void cliInitWriteConfig(void)
{
    int rc;
    FILE *fp = NULL;

    // write Shakefile
    fp = fopen(config.proj_file, "w+");
    if (fp == NULL) {
        LOGE("Failed to open project file for writing: %s", config.proj_file);
        return;
    }
    fprintf(fp, "%s=\"%s\"\n", COMMANDS_DIR, config.cmd_dir);
    fprintf(fp, "%s=\"%s\"\n", COMMANDS_PREFIX, config.cmd_prefix);
    fprintf(fp, "\n\n");
    fprintf(fp, "function cmd-example-func()\n"
            "{\n"
            "    : shake example function, run \"shake example-func\" to execute it\n"
            "    echo \"Welcome to shake!\"\n"
            "    echo\n"
            "    # Shake allows you to pass arguments to script-commands.\n"
            "    # This allows you to easily add flags or options to your script-commands,\n"
            "    # or pass them to another program you use to run.\n"
            "    if (( $# == 0 )); then\n"
            "        echo \"Try passing an argument:\"\n"
            "        echo \"  $ shake example-script hello\"\n"
            "    else\n"
            "        echo \"First argument: \\\"$1\\\"\"\n"
            "    fi\n"
            "    if (( $# == 1 )); then\n"
            "        echo\n"
            "        echo \"You can pass as many arguments (or flags) as you'd like.\"\n"
            "        echo \"Try this:\"\n"
            "        echo \"  $ shake example-script hello --foo\"\n"
            "    elif (( $# > 1 )); then\n"
            "        echo \"Second argument: \\\"$2\\\"\"\n"
            "        echo \"All arguments: $*\"\n"
            "        echo\n"
            "        echo \"To edit this command-script, and for more information, run:\"\n"
            "        echo '  $ shake --edit example-script'\n"
            "        echo\n"
            "        echo \"You can remove this command-script by running:\"\n"
            "        echo \"  $ shake --delete example-script\"\n"
            "        echo\n"
            "        echo \"To create your own command-script, run:\"\n"
            "        echo \"  $ shake --create my-command\"\n"
            "    fi\n"
            "    echo\n"
            "}\n");
    fclose(fp);

    // make cmd dir
    char *cmdDirPath = NULL;
    asprintf(&cmdDirPath, "%s/%s", config.proj_dir, config.cmd_dir);
    check(cmdDirPath != NULL, "asprintf failed");

    rc = mkdir(cmdDirPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    check(rc == 0, "mkdir failed");

    // write example script
    char *scriptPath;
    asprintf(&scriptPath, "%s/%sexample-script", cmdDirPath, config.cmd_prefix);
    if (scriptPath == NULL) {
        LOGE("Failed to create path for example command");
        return;
    }
    fp = fopen(scriptPath, "w+");
    check(fp != NULL, "fopen failed");
    fprintf(fp, "#!/bin/bash\n"
            "# ^ This line is called the shebang, it points to a executable.\n"
            "# When a command is executed it's processed by the shebang executable.\n"
            "# Here are a few example:\n"
            "# nodejs: #!/usr/bin/env node\n"
            "# python: #!/usr/bin/env python\n"
            "# custom: #!/path/to/my/executable\n"
            "# DESC: shake example command, run \"shake example-script\" to execute it\n"
            "echo\n"
            "echo \"Welcome to shake!\"\n"
            "echo\n"
            "# Shake allows you to pass arguments to script-commands.\n"
            "# This allows you to easily add flags or options to your script-commands,\n"
            "# or pass them to another program you use to run.\n"
            "if (( $# == 0 )); then\n"
            "    echo \"Try passing an argument:\"\n"
            "    echo \"  $ shake example-script hello\"\n"
            "else\n"
            "    echo \"First argument: \\\"$1\\\"\"\n"
            "fi\n"
            "if (( $# == 1 )); then\n"
            "    echo\n"
            "    echo \"You can pass as many arguments (or flags) as you'd like.\"\n"
            "    echo \"Try this:\"\n"
            "    echo \"  $ shake example-script hello --foo\"\n"
            "elif (( $# > 1 )); then\n"
            "    echo \"Second argument: \\\"$2\\\"\"\n"
            "    echo \"All arguments: $*\"\n"
            "    echo\n"
            "    echo \"To edit this command-script, and for more information, run:\"\n"
            "    echo '  $ shake --edit example-script'\n"
            "    echo\n"
            "    echo \"You can remove this command-script by running:\"\n"
            "    echo \"  $ shake --delete example-script\"\n"
            "    echo\n"
            "    echo \"To create your own command-script, run:\"\n"
            "    echo \"  $ shake --create my-command\"\n"
            "fi\n"
            "echo\n");
    fclose(fp);

    rc = chmod(scriptPath, S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
    check(rc == 0, "chmod failed");

    free(scriptPath);
error:
    return;
}

int cliInit(void)
{
    if (config.proj_file) {
        char cwd[PATH_MAX];
        check(getcwd(cwd, PATH_MAX) != NULL, "getcwd failed");

        autofree(char) *path = NULL;
        asprintf(&path, "%s/%s", cwd, SHAKEFILE_NAME);
        check_mem(path);

        if (access(path, F_OK) == 0) {
            LOGE("Failed to init");
            LOGW("Path '%s' already configured.", path);
            LOG("To edit te config file, or a specific command, run:");
            LOG("  $ shake --edit [COMMAND]");
            return -1;
        } else {
            LOGN();
            LOGW("Existing shake config file in a parent directory:");
            LOG("  %s", config.proj_file);
        }
    }

    printf("\n");
    printf(ANSI_GRAY("This utility will walk you through the basic shake setup process.\n"));
    printf(ANSI_GRAY("It only covers the required configuration, and suggests typical defaults.\n"));
    printf("\n");
    printf(ANSI_GRAY("Press CTRL-C at any time to quit. No changes are made until you've finished.\n"));
    printf("\n");

    cliInitGetConfig();
    cliInitWriteConfig();

    return 0;

error:
    return -1;
}