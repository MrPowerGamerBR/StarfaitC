#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "stringpointer.h"
#include "arraylist_pathpoint.h"

typedef struct {
    StringPointer name;
    int32_t kind;
    bool closed;
    int32_t precision;
    PathPointArrayList* points;
} Path;
