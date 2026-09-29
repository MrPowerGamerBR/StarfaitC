#pragma once
#include "rvalue.h"
#include "variablecontainer.h"
#include "hashmap_int32_rvalue.h"

typedef struct {
    VariableContainer container;
} GlobalObject;

static inline GlobalObject* GlobalObject_create() {
    GlobalObject* callFrame = calloc(1, sizeof(GlobalObject));
    callFrame->container.variables = Int32RValueHashMap_create(8);

    return callFrame;
}