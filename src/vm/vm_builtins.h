#pragma once
#include "arraylist_builtinfunction.h"
#include "arraylist_builtinvariable.h"

typedef struct {
    BuiltinFunctionArrayList* builtinFunctionsArrayList;
    BuiltinVariableArrayList* builtinVariablesArrayList;
} VMBuiltins;

VMBuiltins* VMBuiltins_create(StarfaitVM* vm);
