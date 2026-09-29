#pragma once
#include "../wad/code.h"
#include "../wad/wad.h"
#include "../wad/variable.h"
#include "vm_stack.h"
#include "callframe.h"

// Forward Declerations
typedef struct StarfaitVM StarfaitVM;

typedef struct {
    const char* name;
    RValue (*builtinFunction)(StarfaitVM*, uint32_t, RValue*);
} BuiltinFunction;

#define ARRAY_LIST_NAME BuiltinFunctionArrayList
#define ARRAY_LIST_TYPE BuiltinFunction
#include "../arraylist.h"
#undef ARRAY_LIST_NAME
#undef ARRAY_LIST_TYPE

#define ARRAY_LIST_NAME VariableArrayList
#define ARRAY_LIST_TYPE Variable
#include "../arraylist.h"
#undef ARRAY_LIST_NAME
#undef ARRAY_LIST_TYPE

struct StarfaitVM {
    GameWAD* wad;
    CallFrame* callFrame;
    VMStack stack;
    BuiltinFunctionArrayList* builtinFunctionsArrayList;
};

StarfaitVM* StarfaitVM_create(GameWAD* wad);

void StarfaitVM_executeBytecodeInstructions(StarfaitVM* vm, StarfaitByteBuffer* buffer);