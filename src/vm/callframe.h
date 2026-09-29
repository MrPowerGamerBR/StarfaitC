#pragma once

#include "variablecontainer.h"

typedef struct {
    VariableContainer container;
    RValue arguments[16];
} CallFrame;