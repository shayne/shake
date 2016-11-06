#include <unistd.h>

#include "dbg.h"

#include "runner.h"

#define MAX_CMD_SIZE 255

extern char **environ;

void Runner_run(char *name, char **argv) {
  char cmd[MAX_CMD_SIZE + 1] = {0};
  int size = snprintf(cmd, MAX_CMD_SIZE + 1, "./scripts/run-%s.sh", name);
  check(size <= MAX_CMD_SIZE,
        "Tried to generate a string larger than buffer: %d", size);

  int rc = execve(cmd, argv, environ);
  check(rc != -1, "execve failed for cmd: %s", cmd);
error:
  return;
}
