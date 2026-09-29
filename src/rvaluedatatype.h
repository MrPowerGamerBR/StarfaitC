#pragma once

#include <stdint.h>
#include <stdlib.h>

#define RVALUE_DATA_TYPES(X) \
    X(RVALUE_DATA_TYPE_UNDEFINED, 0) \
    X(RVALUE_DATA_TYPE_STRING, 1)

#define MAKE_ENUM(name, val) name = val,

typedef enum : uint8_t {
    RVALUE_DATA_TYPES(MAKE_ENUM)
} RValueDataType;

static inline RValueDataType RValueDataType_byId(uint8_t dataType) {
    switch (dataType) {
#define X_CASE(name, val) case val: return val;
        RVALUE_DATA_TYPES(X_CASE)
#undef X_CASE
        default: abort(); // TODO: Add a proper fancy error message
    }
}

static inline const char* RValueDataType_getRValueDataTypeName(uint8_t dataType) {
    switch (dataType) {
#define X_CASE(name, val) case val: return #name;
        RVALUE_DATA_TYPES(X_CASE)
#undef X_CASE
        default: abort(); // TODO: Add a proper fancy error message
    }
}