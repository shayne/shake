//
// Created by shayne on 11/15/16.
//

#ifndef SHAKE_COMMANDS_H
#define SHAKE_COMMANDS_H

#include <unistd.h>

ssize_t loadCommands(char ***cmds_out, char ***descs_out);
void loadCommandsSorted(void);

#endif  // SHAKE_COMMANDS_H
