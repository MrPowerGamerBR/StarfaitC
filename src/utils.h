#pragma once

#include <stdint.h>
#include <stddef.h>

#define repeat(i, n) for (typeof(i) n = 0; i > n; n++)

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