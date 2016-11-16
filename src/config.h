//
// Created by shayne on 11/14/16.
//

#ifndef SHAKE_CONFIG_H
#define SHAKE_CONFIG_H

#define DEFAULT_CMD_DIR "scripts"
#define DEFAULT_CMD_PREFIX "cmd-"

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

void loadConfig(char *filename);

#endif // SHAKE_CONFIG_H
