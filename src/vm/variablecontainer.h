#pragma once
#include "rvalue.h"
#include "../int_hashmap.h"

typedef struct {
    Int2RValueHashMap* variables;
} VariableContainer;

static inline void VariableContainer_setVariable(VariableContainer* container, uint32_t variableId, RValue value) {
    printf("Setting variable %d\n", variableId);

    Int2RValueHashMap_put(container->variables, variableId, value);
}

static inline RValue VariableContainer_getVariable(VariableContainer* container, uint32_t variableId) {
    printf("Getting variable %d\n", variableId);

    return *Int2RValueHashMap_get(container->variables, variableId);
}