#pragma once

#include <arraylist_rvalue.h>

#include "variablecontainer.h"

typedef struct {
    VariableContainer container;
    // TODO: *Maybe* this should be a HashMap, considering that games can write directly to a specific argument index (like argument3 = "blah")
    RValueArrayList* arguments;
} CallFrame;

static inline CallFrame* CallFrame_create() {
    CallFrame* callFrame = calloc(1, sizeof(CallFrame));
    callFrame->container.variables = Int2RValueHashMap_create(8);

    callFrame->arguments = RValueArrayList_create(16);
    repeat(16, i) {
        RValueArrayList_add(callFrame->arguments, RValue_createUndefined());
    }

    return callFrame;
}

static inline void CallFrame_free(CallFrame* callFrame) {
    // TODO: this
}