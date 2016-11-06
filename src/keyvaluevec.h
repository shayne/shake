#ifndef _keyvalue_vec
#define _keyvalue_vec

#include <stdlib.h>

typedef struct KeyValueNode {
    char *key;
    void *value;
} KeyValueNode;

typedef struct KeyValueVec {
    KeyValueNode *cursor;
    KeyValueNode *nodes;
} KeyValueVec;

KeyValueVec *KeyValueVec_create(size_t count);
void KeyValueVec_destroy(KeyValueVec *vec);

void KeyValueVec_set(KeyValueVec *vec, char *key, void *value);

#endif
