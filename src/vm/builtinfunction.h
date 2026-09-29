#pragma once
#include "rvalue.h"

// Forward Declerations
typedef struct StarfaitVM StarfaitVM;

typedef struct {
    const char* name;
    RValue (*builtinFunction)(StarfaitVM*, uint32_t, RValue*);
} BuiltinFunction;