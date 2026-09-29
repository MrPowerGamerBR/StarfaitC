#pragma once
#include "rvalue.h"

typedef struct {
    RValue variables[64];
} VariableContainer;

static inline void VariableContainer_setVariable(VariableContainer* container, uint32_t variableId, RValue value) {
    printf("Setting variable %d\n", variableId);
    container->variables[variableId] = value;
}

static inline RValue VariableContainer_getVariable(VariableContainer* container, uint32_t variableId) {
    printf("Getting variable %d\n", variableId);
    return container->variables[variableId];
}