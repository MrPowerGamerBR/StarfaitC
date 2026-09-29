#pragma once
#include <stdint.h>

#define OPCODES(X)          \
    X(OP_PUSH, 0xC0)        \
    X(OP_CALL, 0xD9)        \
    X(OP_POPZ, 0x9E)

#define MAKE_ENUM(name, val) name = val,
typedef enum : uint8_t {
    OPCODES(MAKE_ENUM)
} Opcode;

static inline const char* Op_getOpcodeName(Opcode op) {
    switch (op) {
#define X_CASE(name, val) case val: return #name;
        OPCODES(X_CASE)
#undef X_CASE
        default: return "UNKNOWN";
    }
}