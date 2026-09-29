#pragma once

#include <stdint.h>
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    int32_t occurrenceCount;
    int32_t firstAddress;
} Function;
