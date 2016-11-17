//
// Created by shayne on 11/17/16.
//

#ifndef SHAKE_UTIL_H
#define SHAKE_UTIL_H

#define DEF_AUTOFREE(N, C)                                                                         \
        static inline void _autofree_func_##N(void *p)                                             \
        {                                                                                          \
                if (p && *(N **)p) {                                                               \
                        C(*(N **)p);                                                               \
                        (*(void **)p) = NULL;                                                      \
                }                                                                                  \
        }

#define autofree(N) __attribute__((cleanup(_autofree_func_##N))) N

DEF_AUTOFREE(char, free)

#endif //SHAKE_UTIL_H
