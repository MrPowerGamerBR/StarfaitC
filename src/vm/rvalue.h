#pragma once

#include "rvaluedatatype.h"
#include "starfaitstring.h"
#include "../utils.h"

typedef struct {
    RValueDataType type;
    union {
        int32_t int32;
        StarfaitString* string;
        double real;
        bool boolean;
    } value;
} RValue;

static inline RValue RValue_createUndefined() {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_UNDEFINED
    };
}

static inline RValue RValue_createString(StarfaitString* string) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_STRING,
        .value = { .string = string }
    };
}

static inline RValue RValue_createStringFromCStringCopy(char* string) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_STRING,
        .value = { .string = StarfaitString_create(string) }
    };
}

static inline RValue RValue_createStringFromCStringCopyAndFree(char* string) {
    RValue rvalue = RValue_createStringFromCStringCopy(string);
    free(string);
    return rvalue;
}

static inline RValue RValue_createInt32(int32_t value) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_INT32,
        .value = { .int32 = value }
    };
}

static inline RValue RValue_createBoolean(bool value) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_BOOLEAN,
        .value = { .boolean = value }
    };
}

static inline RValue RValue_createReal(int32_t value) {
    return (RValue) {
        .type = RVALUE_DATA_TYPE_REAL,
        .value = { .real = value }
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
        case RVALUE_DATA_TYPE_STRING: return strdup(StarfaitString_toCharArrayView(rvalue.value.string));
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
        case RVALUE_DATA_TYPE_BOOLEAN:
            return strdup(rvalue.value.boolean ? "1" : "0");
    }
    abort();
}

static inline double RValue_getAsReal(RValue rvalue) {
    switch (rvalue.type) {
        case RVALUE_DATA_TYPE_UNDEFINED: TODO();
        case RVALUE_DATA_TYPE_STRING: TODO();
        case RVALUE_DATA_TYPE_INT32: return rvalue.value.int32;
        case RVALUE_DATA_TYPE_BOOLEAN: TODO();
        case RVALUE_DATA_TYPE_REAL: return rvalue.value.real;
    }
    abort();
}

static inline bool RValue_getAsBoolean(RValue rvalue) {
    switch (rvalue.type) {
        case RVALUE_DATA_TYPE_UNDEFINED: TODO();
        case RVALUE_DATA_TYPE_STRING: TODO();
        case RVALUE_DATA_TYPE_INT32: TODO();
        case RVALUE_DATA_TYPE_BOOLEAN: return rvalue.value.boolean;
        // GameMaker-HTML5's yyGetBool returns true if the value is > 0.5
        case RVALUE_DATA_TYPE_REAL: return rvalue.value.real > 0.5;
    }
    abort();
}

static inline void RValue_free(RValue rvalue) {
    switch (rvalue.type) {
        case RVALUE_DATA_TYPE_UNDEFINED: break;
        case RVALUE_DATA_TYPE_STRING: StarfaitString_free(rvalue.value.string);
        case RVALUE_DATA_TYPE_INT32: break;
        case RVALUE_DATA_TYPE_BOOLEAN: break;
        case RVALUE_DATA_TYPE_REAL: break;
    }
}

static inline RValue RValue_createCopy(RValue rvalue) {
    switch (rvalue.type) {
        case RVALUE_DATA_TYPE_UNDEFINED: return RValue_createUndefined();
        case RVALUE_DATA_TYPE_STRING: return RValue_createStringFromCStringCopy(StarfaitString_toCharArrayView(rvalue.value.string));
        case RVALUE_DATA_TYPE_INT32: return RValue_createInt32(rvalue.value.int32);
        case RVALUE_DATA_TYPE_BOOLEAN: return RValue_createBoolean(rvalue.value.boolean);
        case RVALUE_DATA_TYPE_REAL: return RValue_createReal(rvalue.value.real);
    }
    abort();
}