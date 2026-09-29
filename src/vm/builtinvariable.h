#pragma once
#include "rvalue.h"
#include "vm_forward.h"

typedef struct {
    const char* name;
    RValue (*builtinVariableReader)(StarfaitVM*, int32_t, intptr_t);
    intptr_t userData;
} BuiltinVariable;