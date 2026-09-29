#pragma once
#include "../wad/code.h"
#include "../wad/wad.h"
#include "vm_stack.h"
#include "callframe.h"
#include "arraylist_builtinfunction.h"
#include "arraylist_variable.h"

struct StarfaitVM {
    GameWAD* wad;
    CallFrame* callFrame;
    VMStack stack;
    BuiltinFunctionArrayList* builtinFunctionsArrayList;
};

StarfaitVM* StarfaitVM_create(GameWAD* wad);

RValue StarfaitVM_executeCode(StarfaitVM* vm, CodeEntry* code);