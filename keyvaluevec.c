#include <stdlib.h>
#include <strings.h>

#include "dbg.h"

#include "keyvaluevec.h"

KeyValueVec *KeyValueVec_create(size_t count)
{
    KeyValueVec *vec = calloc(1, sizeof(KeyValueVec));
    check_mem(vec);

    KeyValueNode *nodes = calloc(count + 1, sizeof(KeyValueNode));
    check_mem(nodes);

    vec->nodes = nodes;
    vec->cursor = nodes;

    return vec;

error:
    if (vec->nodes)
        free(vec->nodes);
    if (vec)
        free(vec);
    return NULL;
}

void KeyValueVec_destroy(KeyValueVec *vec)
{
    if (vec->nodes)
        free(vec->nodes);
    free(vec);
}

void KeyValueVec_set(KeyValueVec *vec, char *key, char *value)
{
    KeyValueNode *node = vec->cursor;
    check(node != NULL, "No node to set to");

    node->key = key;
    node->value = value;

    vec->cursor++;

error:
    return;
}
