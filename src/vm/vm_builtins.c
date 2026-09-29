#include "vm_builtins.h"

#include "vm.h"

static RValue builtin_show_debug_message([[maybe_unused]] StarfaitVM* vm, [[maybe_unused]] int32_t argCount, RValue* args) {
    printf("Game: %s\n", RValue_toString(args[0]));

    return RValue_createUndefined();
}

static RValue builtin_string([[maybe_unused]] StarfaitVM* vm, [[maybe_unused]] int32_t argCount, RValue* args) {
    return RValue_createStringFromCStringCopyAndFree(RValue_toString(args[0]));
}

static RValue variable_reader_argumentN(StarfaitVM* vm, [[maybe_unused]] int32_t arrayIndex, intptr_t userData) {
    CallFrame* currentCallFrame = StarfaitVM_getCurrentCallFrame(vm);
    return RValue_createCopy(*RValueArrayList_get(currentCallFrame->arguments, (int32_t) userData));
}

static RValue variable_reader_argument_count(StarfaitVM* vm, [[maybe_unused]] int32_t arrayIndex, [[maybe_unused]] intptr_t userData) {
    CallFrame* currentCallFrame = StarfaitVM_getCurrentCallFrame(vm);
    return RValue_createReal(currentCallFrame->arguments->size);
}

static void registerBuiltinFunction(VMBuiltins* builtins, const char* name, RValue (*builtinFunction)(StarfaitVM*, int32_t, RValue*)) {
    BuiltinFunctionArrayList_add(
        builtins->builtinFunctionsArrayList,
        (BuiltinFunction){
            .name = name,
            .builtinFunction = builtinFunction
        }
    );
}

static void registerBuiltinVariable(VMBuiltins* builtins, const char* name, RValue (*builtinVariableReader)(StarfaitVM*, int32_t, intptr_t), intptr_t userData) {
    BuiltinVariableArrayList_add(
        builtins->builtinVariablesArrayList,
        (BuiltinVariable){
            .name = name,
            .builtinVariableReader = builtinVariableReader,
            .userData = userData
        }
    );
}

VMBuiltins* VMBuiltins_create(StarfaitVM* vm) {
    VMBuiltins* builtins = calloc(1, sizeof(VMBuiltins));
    builtins->builtinFunctionsArrayList = BuiltinFunctionArrayList_create(8);
    builtins->builtinVariablesArrayList = BuiltinVariableArrayList_create(8);

    registerBuiltinFunction(builtins, "show_debug_message", builtin_show_debug_message);
    registerBuiltinFunction(builtins, "string", builtin_string);

    repeat(16, i) {
        char name[16];
        snprintf(name, sizeof(name), "argument%d", i);
        registerBuiltinVariable(builtins, strdup(name), variable_reader_argumentN, i);
    }

    registerBuiltinVariable(builtins, "argument_count", variable_reader_argument_count, 0);

    return builtins;
}