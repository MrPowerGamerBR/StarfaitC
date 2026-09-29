#pragma once
#include "rvalue.h"

#define HASH_MAP_NAME Int2RValueHashMap
#define HASH_MAP_TYPE RValue
#include "../int32_hashmap.h"
#undef HASH_MAP_NAME
#undef HASH_MAP_TYPE

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