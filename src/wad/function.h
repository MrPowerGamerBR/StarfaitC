#pragma once

#include <stdint.h>
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    uint32_t occurrenceCount;
    uint32_t firstAddress;
} Function;
