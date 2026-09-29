#pragma once
#include <stdint.h>

#define VARIABLE_SCOPES(X) \
    X(VARIABLE_SCOPE_SELF, -1) \
    X(VARIABLE_SCOPE_OTHER, -2) \
    X(VARIABLE_SCOPE_GLOBAL, -5) \
    X(VARIABLE_SCOPE_LOCAL, -7)

#define MAKE_ENUM(name, val) name = val,
typedef enum : int8_t {
    VARIABLE_SCOPES(MAKE_ENUM)
} VariableScope;

static inline VariableScope VariableScope_byId(int32_t variableScope) {
    switch (variableScope) {
#define X_CASE(name, val) case val: return val;
        VARIABLE_SCOPES(X_CASE)
#undef X_CASE
        default: abort(); // TODO: Add a proper fancy error message
    }
}

static inline const char* VariableScope_getVariableScopeName(VariableScope variableScope) {
    switch (variableScope) {
#define X_CASE(name, val) case val: return #name;
        VARIABLE_SCOPES(X_CASE)
#undef X_CASE
        default: return "UNKNOWN";
    }
}