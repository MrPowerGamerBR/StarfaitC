#pragma once
#include "rvalue.h"

typedef struct {
    RValue variables[64];
} VariableContainer;

static inline void VariableContainer_setVariable(VariableContainer* container, uint32_t variableId, RValue value) {
    container->variables[variableId] = value;
}

static inline RValue VariableContainer_getVariable(VariableContainer* container, uint32_t variableId) {
    return container->variables[variableId];
}