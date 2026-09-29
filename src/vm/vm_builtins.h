#pragma once
#include "vm.h"

typedef struct {
    RValue (*builtinFunction)(StarfaitVM*, uint32_t, RValue*);
} BuiltinFunction;

void VMBuiltins_registerBuiltins(StarfaitVM* vm);
