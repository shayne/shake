//
// Created by shayne on 11/14/16.
//

#ifndef SHAKE_CONFIG_H
#define SHAKE_CONFIG_H

extern char const *SHAKEFILE_NAME;
extern char const *COMMANDS_DIR;
extern char const *COMMANDS_PREFIX;

extern char const *DEFAULT_CMD_DIR;
extern char const *DEFAULT_CMD_PREFIX;

struct shakeConfig {
    char *proj_file;
    char *proj_dir;
    char *proj_dir_s;
    char *cmd_dir;
    int cmd_dir_len;
    char *cmd_prefix;
    int cmd_prefix_len;
};

extern struct shakeConfig config;

void initConfig(void);
void destroyConfig(void);

void loadConfig(void);

#endif  // SHAKE_CONFIG_H
