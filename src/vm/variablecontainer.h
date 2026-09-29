#pragma once
#include "rvalue.h"
#include "hashmap_int32_rvalue.h"

typedef struct {
    Int32RValueHashMap* variables;
} VariableContainer;

static inline void VariableContainer_setVariable(VariableContainer* container, uint32_t variableId, RValue value) {
    printf("Setting variable %d\n", variableId);

    Int32RValueHashMap_put(container->variables, variableId, value);
}

static inline RValue VariableContainer_getVariable(VariableContainer* container, uint32_t variableId) {
    printf("Getting variable %d\n", variableId);

    return *Int32RValueHashMap_get(container->variables, variableId);
}