#pragma once

#include <arraylist_rvalue.h>
#include "variablecontainer.h"
#include "hashmap_int32_rvalue.h"

typedef struct CallFrame CallFrame;

struct CallFrame {
    CallFrame* previous;
    VariableContainer container;
    // TODO: *Maybe* this should be a HashMap, considering that games can write directly to a specific argument index (like argument3 = "blah")
    RValueArrayList* arguments;
};

static inline CallFrame CallFrame_create() {
    return (CallFrame) {
        .container.variables = Int32RValueHashMap_create(8),
        .arguments = RValueArrayList_create(16)
    };
}

static inline void CallFrame_free(CallFrame* callFrame) {
    // TODO: this
}