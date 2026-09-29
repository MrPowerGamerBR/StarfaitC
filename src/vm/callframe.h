#pragma once

#include <arraylist_rvalue.h>
#include "variablecontainer.h"
#include "hashmap_int32_rvalue.h"

typedef struct {
    VariableContainer container;
    // TODO: *Maybe* this should be a HashMap, considering that games can write directly to a specific argument index (like argument3 = "blah")
    RValueArrayList* arguments;
} CallFrame;

static inline CallFrame* CallFrame_create() {
    CallFrame* callFrame = calloc(1, sizeof(CallFrame));
    callFrame->container.variables = Int32RValueHashMap_create(8);
    callFrame->arguments = RValueArrayList_create(16);

    return callFrame;
}

static inline void CallFrame_free(CallFrame* callFrame) {
    // TODO: this
}