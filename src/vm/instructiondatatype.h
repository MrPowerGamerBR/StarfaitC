#pragma once

#include <stdint.h>

#define INSTRUCTION_DATA_TYPES(X) \
    X(DATA_TYPE_DOUBLE, 0) \
    X(DATA_TYPE_FLOAT, 1) \
    X(DATA_TYPE_INT32, 2) \
    X(DATA_TYPE_INT64, 3) \
    X(DATA_TYPE_BOOLEAN, 4) \
    X(DATA_TYPE_VARIABLE, 5) \
    X(DATA_TYPE_STRING, 6) \
    X(DATA_TYPE_INT16, 15)

#define MAKE_ENUM(name, val) name = val,
typedef enum : uint8_t {
    INSTRUCTION_DATA_TYPES(MAKE_ENUM)
} InstructionDataType;

static inline InstructionDataType InstructionDataType_byId(uint8_t dataType) {
    switch (dataType) {
#define X_CASE(name, val) case val: return val;
        INSTRUCTION_DATA_TYPES(X_CASE)
#undef X_CASE
        default: abort(); // TODO: Add a proper fancy error message
    }
}

static inline const char* InstructionDataType_getDataTypeName(InstructionDataType dataType) {
    switch (dataType) {
#define X_CASE(name, val) case val: return #name;
        INSTRUCTION_DATA_TYPES(X_CASE)
#undef X_CASE
        default: return "UNKNOWN";
    }
}