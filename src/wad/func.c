#include "func.h"

#include <stdio.h>
#include <stdlib.h>

#include "../starfaitbytebuffer.h"
#include "../utils.h"

FUNC FUNC_parse(StarfaitByteBuffer* buffer) {
    size_t count = StarfaitByteBuffer_readUint32LE(buffer);
    Function* functions = calloc(count, sizeof(Function));

    repeat(count, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        uint32_t occurrenceCount = StarfaitByteBuffer_readUint32LE(buffer);
        uint32_t firstAddress = StarfaitByteBuffer_readUint32LE(buffer);

        functions[i].name = name;
        functions[i].occurrenceCount = occurrenceCount;
        functions[i].firstAddress = firstAddress;
    }

    return (FUNC) {
        .functionCount = count,
        .functions = functions,
    };
}