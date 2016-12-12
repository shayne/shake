//
// Created by shayne on 11/17/16.
//

#ifndef SHAKE_LOG_H
#define SHAKE_LOG_H

#include "colors.h"

#define LOG(...)                  \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n");

#define LOGN() fprintf(stderr, "\n");

#define LOGE(...)                                         \
    fprintf(stderr, COLOR_ERROR("[ERROR] ") __VA_ARGS__); \
    fprintf(stderr, "\n");

#define LOGW(...)                                       \
    fprintf(stderr, COLOR_WARN("[WARN] ") __VA_ARGS__); \
    fprintf(stderr, "\n");

#endif  // SHAKE_LOG_H
