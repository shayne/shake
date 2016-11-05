#include <stdlib.h>
#include <strings.h>

#include "dbg.h"

#include "keyvaluevec.h"


KeyValueVec * KeyValueVec_create(unsigned int count)
{
    int i = 0;

    KeyValueVec *vec = calloc(1, sizeof(KeyValueVec));
    check_mem(vec);

    KeyValueNode * nodes = calloc(count + 1, sizeof(KeyValueNode));
    check_mem(nodes);

    vec->nodes = nodes;
    vec->cursor = nodes;

    return vec;

error:
    if (vec->nodes) free(vec->nodes);
    if (vec) free(vec);
    return NULL;
}

void KeyValueVec_destroy(KeyValueVec * vec)
{
    if (vec->nodes) {
        KeyValueNode * nodesptr = vec->nodes;
        for (; nodesptr && nodesptr->key != NULL; nodesptr++) {
            if (nodesptr->key) free(nodesptr->key);
            if (nodesptr->value) free(nodesptr->value);
        }
        free(vec->nodes);
    }
    free(vec);
}

void KeyValueVec_set(KeyValueVec * vec, char *key, char *value)
{
    KeyValueNode *node = vec->cursor;
    check(node != NULL, "No node to set to");

    node->key = calloc(strlen(key) + 1, sizeof(char));
    node->value = calloc(strlen(value) + 1, sizeof(char));

    strncpy(node->key, key, strlen(key));
    strncpy(node->value, value, strlen(value));

    vec->cursor++;

error:
    return;
}
