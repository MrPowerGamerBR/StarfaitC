#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define repeat(i, n) for (typeof(i) n = 0; i > n; n++)

#define require(cond, ...)                                              \
    do {                                                                \
        if (!(cond)) [[unlikely]] {                                     \
            fprintf(stderr, "%s:%d: %s: require failed: %s",            \
                    __FILE__, __LINE__, __func__, #cond);               \
            __VA_OPT__(fprintf(stderr, " - ");                          \
                       fprintf(stderr, __VA_ARGS__);)                   \
            fputc('\n', stderr);                                        \
            abort();                                                    \
        }                                                               \
    } while (0)

static inline uint32_t Utils_minFromArray(const uint32_t* elements, size_t size) {
    // TODO: abort if size is 0
    uint32_t minAddressTarget = INT32_MAX;

    repeat(size, i) {
        if (minAddressTarget > elements[i]) {
            minAddressTarget = elements[i];
        }
    }

    return minAddressTarget;
}
