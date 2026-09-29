#pragma once
#include "../wad/code_chunk.h"
#include "../wad/wad.h"
#include "vm_stack.h"
#include "callframe.h"
#include "vm_builtins.h"
#include "vm_forward.h"
#include "arraylist_rvalue.h"
#include "arraylist_callframe.h"
#include "arraylist_string.h"
#include "globalobject.h"

struct StarfaitVM {
    GameWAD* wad;
    CallFrameArrayList* callFrameStack;
    GlobalObject* global;
    StringArrayList* regularVariableNames;
    VMStack stack;
    VMBuiltins* builtins;
};

StarfaitVM* StarfaitVM_create(GameWAD* wad);

/**
 * Returns the current CallFrame
 *
 * @param vm the StarfaitVM instance
 * @return the current CallFrame
 */
CallFrame* StarfaitVM_getCurrentCallFrame(StarfaitVM* vm);
RValue StarfaitVM_executeCode(StarfaitVM* vm, CodeEntry* code, RValueArrayList* arguments);