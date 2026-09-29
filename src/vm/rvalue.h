#pragma once

#include "rvaluedatatype.h"
#include "../utils.h"

typedef struct {
    RValueDataType type;
    union {
        int32_t int32;
        char* string;
        double real;
    } value;
    // If true, then you should free any of the values when the value gets out of scope
    bool isOwned;
} RValue;

static inline RValue RValue_createUndefined() {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_UNDEFINED,
        .isOwned = false
    };
}

static inline RValue RValue_createReferencedString(char* string) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_STRING,
        .value = { .string = string },
        .isOwned = false
    };
}

static inline RValue RValue_createOwnedString(char* string) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_STRING,
        .value = { .string = string },
        .isOwned = true
    };
}

static inline RValue RValue_createInt32(int32_t value) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_INT32,
        .value = { .int32 = value },
        .isOwned = false
    };
}

static inline RValue RValue_createReal(int32_t value) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_REAL,
        .value = { .real = value },
        .isOwned = false
    };
}


/**
 * Converts the RValue to a String
 *
 * The result MUST be freed!
 *
 * @param rvalue the input
 * @return the RValue as a string
 */
static inline char* RValue_toString(RValue rvalue) {
    switch (rvalue.type) {
        case RVALUE_DATA_TYPE_UNDEFINED: return strdup("undefined");
        case RVALUE_DATA_TYPE_STRING: return strdup(rvalue.value.string);
        case RVALUE_DATA_TYPE_INT32: {
            char buf[12];
            snprintf(buf, sizeof(buf), "%d", rvalue.value.int32);
            return strdup(buf);
        }
        case RVALUE_DATA_TYPE_REAL: {
            // Truncate value, if it is the same as the long result, then we don't need to render decimal places
            uint64_t asLong = (uint64_t) rvalue.value.real;
            if (asLong == rvalue.value.real) {
                char buf[12];
                snprintf(buf, sizeof(buf), "%lu", asLong);
                return strdup(buf);
            }

            char buf[12];
            snprintf(buf, sizeof(buf), "%.2f", rvalue.value.real);
            return strdup(buf);
        }
    }
    abort();
}