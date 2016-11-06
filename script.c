#include <stdlib.h>
#include <string.h>

#include "bstrlib.h"

#include "dbg.h"

#include "script.h"

#define MAX 255
#define SCRIPT_PREFIX "scripts/run-"

static struct tagbstring bprefix = bsStatic(SCRIPT_PREFIX);
static struct tagbstring bdot = bsStatic(".");

bstring Script_makename(bstring path)
{
    int pos = 0;
    pos = binstr(path, 0, &bprefix);
    check(pos != BSTR_ERR, "Failed to find prefix");
    check(pos == 0, "Prefix position should be 0");
    int left = blength(&bprefix);

    pos = binstrr(path, blength(path) - 1, &bdot);
    check(pos != BSTR_ERR, "Failed to find dot");
    int dpos = pos;

    return bmidstr(path, left, dpos - left);

error:
    return NULL;
}

Script *Script_create(char *path)
{
    Script *out = NULL;
    out = calloc(1, sizeof(Script));
    check_mem(out);

    out->path = bfromcstr(path);
    out->name = Script_makename(out->path);
    check_mem(out->name);

    return out;

error:
    if (out->path)
        bdestroy(out->path);
    if (out->name)
        bdestroy(out->name);
    if (out)
        free(out);
    return NULL;
}

void Script_destroy(Script *script)
{
    bdestroy(script->name);
    bdestroy(script->path);
    free(script);
}

char *Script_name(Script *script)
{
    return bdata((bstring)script->name);
}

char *Script_path(Script *script)
{
    return bdata((bstring)script->path);
}
