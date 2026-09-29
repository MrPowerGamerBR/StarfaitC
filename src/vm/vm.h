#pragma once
#include "../wad/code.h"
#include "../wad/wad.h"
#include "vm_stack.h"
#include "callframe.h"
#include "vm_builtins.h"
#include "vm_forward.h"
#include "arraylist_rvalue.h"
#include "arraylist_callframe.h"

struct StarfaitVM {
    GameWAD* wad;
    CallFrameArrayList* callFrameStack;
    VMStack stack;
    VMBuiltins* builtins;
};

StarfaitVM* StarfaitVM_create(GameWAD* wad);

CallFrame* StarfaitVM_getCurrentCallFrame(StarfaitVM* vm);
RValue StarfaitVM_executeCode(StarfaitVM* vm, CodeEntry* code, RValueArrayList* arguments);