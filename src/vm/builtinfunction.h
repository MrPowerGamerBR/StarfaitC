#pragma once
#include "rvalue.h"
#include "vm_forward.h"

typedef struct {
    const char* name;
    RValue (*builtinFunction)(StarfaitVM*, int32_t, RValue*);
} BuiltinFunction;