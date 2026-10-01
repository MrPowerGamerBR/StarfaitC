#pragma once
#include "arraylist_builtinfunction.h"
#include "arraylist_builtinvariable.h"

typedef struct {
    BuiltinFunctionArrayList* builtinFunctionsArrayList;
    BuiltinVariableArrayList* builtinVariablesArrayList;
} VMBuiltins;

void VMBuiltins_init(VMBuiltins* builtins);
