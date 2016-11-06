#ifndef _script_h
#define _script_h

typedef struct Script {
    void *name;
    void *path;
} Script;

Script *Script_create(char *path);
Script *Script_createfromname(char *name);

void Script_destroy(Script *script);

char *Script_name(Script *script);
char *Script_path(Script *script);

#endif
