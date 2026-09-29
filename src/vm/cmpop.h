#pragma once
#include <stdint.h>

#define CMP_OPCODES(X)                \
    X(CMPOP_LESS_THAN, 1)             \
    X(CMPOP_LESS_THAN_OR_EQUAL, 2)    \
    X(CMPOP_EQUAL, 3)                 \
    X(CMPOP_NOT_EQUAL, 4)             \
    X(CMPOP_GREATER_THAN_OR_EQUAL, 5) \
    X(CMPOP_GREATER_THAN, 6)

#define MAKE_ENUM(name, val) name = val,
typedef enum : uint8_t {
    CMP_OPCODES(MAKE_ENUM)
} CmpOp;

static inline const char* CmpOp_getCmpOpcodeName(CmpOp op) {
    switch (op) {
#define X_CASE(name, val) case val: return #name;
        CMP_OPCODES(X_CASE)
#undef X_CASE
        default: return "UNKNOWN";
    }
}