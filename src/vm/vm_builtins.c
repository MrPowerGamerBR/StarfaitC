#include "vm_builtins.h"

RValue builtin_show_debug_message([[maybe_unused]] StarfaitVM* vm, [[maybe_unused]] uint32_t argCount, RValue* args) {
    printf("Game: %s\n", RValue_toString(args[0]));

    return RValue_createUndefined();
}

void registerBuiltin(StarfaitVM* vm, const char* name, RValue (*builtinFunction)(StarfaitVM*, uint32_t, RValue*)) {
    BuiltinFunctionArrayList_add(
        vm->builtinFunctionsArrayList,
        (BuiltinFunction){
            .name = name,
            .builtinFunction = builtinFunction
        }
    );
}

void VMBuiltins_registerBuiltins(StarfaitVM* vm) {
    registerBuiltin(vm, "show_debug_message", builtin_show_debug_message);
}
