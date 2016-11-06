#ifndef _keyvalue_vec
#define _keyvalue_vec

typedef struct KeyValueNode {
  char *key;
  char *value;
} KeyValueNode;

typedef struct KeyValueVec {
  KeyValueNode *cursor;
  KeyValueNode *nodes;
} KeyValueVec;

KeyValueVec *KeyValueVec_create(size_t);
void KeyValueVec_destroy(KeyValueVec *vec);

void KeyValueVec_set(KeyValueVec *vec, char *key, char *value);

#endif
