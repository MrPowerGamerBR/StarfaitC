#include "vm_builtins.h"

RValue builtin_show_debug_message(StarfaitVM* vm, uint32_t argCount, RValue* args) {
    printf("I was called yippee\n");

    return RValue_createUndefined();
}

void registerBuiltin(StarfaitVM* vm, const char* name, RValue (*builtinFunction)(StarfaitVM*, uint32_t, RValue*)) {
    // builtinFunction(vm);
}

void VMBuiltins_registerBuiltins(StarfaitVM* vm) {
    registerBuiltin(vm, "show_debug_message", builtin_show_debug_message);
}