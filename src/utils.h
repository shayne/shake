//
// Created by shayne on 11/8/16.
//

#ifndef SHAKE_UTILS_H
#define SHAKE_UTILS_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    char **array;
    size_t used;
    size_t size;
} CharPArray;

static inline void capinit(CharPArray *ca, size_t initial)
{
    ca->array = (char **)calloc(initial, sizeof(char *));
    ca->used = 0;
    ca->size = initial;
}

static inline void capinsert(CharPArray *ca, char *data, size_t size)
{
    if (ca->used == ca->size) {
        ca->size *= 2;
        ca->array = (char **)realloc(ca->array, ca->size * sizeof(char *));
    }
    ca->array[ca->used++] = strndup(data, size);
}

static inline void capfree(CharPArray *ca)
{
    int i;
    for (i = 0; i < ca->used; i++) {
        free(ca->array[i]);
    }
    free(ca->array);
    ca->array = NULL;
    ca->used = ca->size = 0;
}

#endif // SHAKE_UTILS_H
