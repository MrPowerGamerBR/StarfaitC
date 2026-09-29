#pragma once
#include <stdint.h>

#define OPCODES(X)          \
    X(OP_PUSH, 0xC0)        \
    X(OP_CALL, 0xD9)        \
    X(OP_POPZ, 0x9E)

#define X_CONST(name, val) constexpr uint8_t name = val;
OPCODES(X_CONST)

const char *Op_getOpcodeName(uint8_t op) {
    switch (op) {
#define X_CASE(name, val) case val: return #name;
        OPCODES(X_CASE)
#undef X_CASE
        default: return "UNKNOWN";
    }
}