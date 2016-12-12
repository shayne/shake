#ifndef __dbg_h__
#define __dbg_h__

#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#define debug(...)
#else
#define debug(...)                                        \
    fprintf(stderr, "DEBUG %s:%d: ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                         \
    fprintf(stderr, "\n")
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(...)                                                   \
    fprintf(stderr, "[ERROR] (%s:%d: errno: %s) ", __FILE__, __LINE__, \
            clean_errno());                                            \
    fprintf(stderr, __VA_ARGS__);                                      \
    fprintf(stderr, "\n");

#define log_warn(...)                                                 \
    fprintf(stderr, "[WARN] (%s:%d: errno: %s) ", __FILE__, __LINE__, \
            clean_errno());                                           \
    fprintf(stderr, __VA_ARGS__);                                     \
    fprintf(stderr, "\n")

#define log_info(...)                                       \
    fprintf(stderr, "[INFO] (%s:%d) ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                           \
    fprintf(stderr, "\n")

#define check(A, ...)         \
    if (!(A)) {               \
        log_err(__VA_ARGS__); \
        errno = 0;            \
        goto error;           \
    }

#define sentinel(...)         \
    {                         \
        log_err(__VA_ARGS__); \
        errno = 0;            \
        goto error;           \
    }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, ...) \
    if (!(A)) {             \
        debug(__VA_ARGS__); \
        errno = 0;          \
        goto error;         \
    }

#endif
