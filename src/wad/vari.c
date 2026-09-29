#include "gen8.h"

#include <stdio.h>
#include <stdlib.h>

#include "../arraylist.h"
#include "../charutils.h"
#include "../utils.h"
#include "variable.h"
#include "vari.h"

constexpr uint32_t ENTRY_SIZE = 20;

VARI VARI_parse(StarfaitByteBuffer* buffer, size_t chunkSize) {
    uint32_t start = buffer->position;
    uint32_t globalVariables = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t instanceVariables = StarfaitByteBuffer_readUint32LE(buffer); // Somehow it seems that modern GM games always have globalVariables == instanceVariables
    uint32_t localVariables = StarfaitByteBuffer_readUint32LE(buffer);

    // The WAD doesn't give us the entry count (fun!)
    uint32_t remaining = ((start + chunkSize) - buffer->position);
    uint32_t entryCount = remaining / ENTRY_SIZE;
    Variable* variables = calloc(entryCount, sizeof(Variable));

    repeat(entryCount, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        uint32_t instanceType = StarfaitByteBuffer_readUint32LE(buffer);
        int32_t varId = StarfaitByteBuffer_readInt32LE(buffer);
        uint32_t occurrenceCount = StarfaitByteBuffer_readUint32LE(buffer);
        int32_t firstAddress = StarfaitByteBuffer_readInt32LE(buffer);

        variables[i].name = name;
        variables[i].instanceType = instanceType;
        variables[i].varId = varId;
        variables[i].occurrenceCount = occurrenceCount;
        variables[i].firstAddress = firstAddress;
    }

    return (VARI) {
        .globalVariables = globalVariables,
        .instanceVariables = instanceVariables,
        .localVariables = localVariables,
        .variableCount = entryCount,
        .variables = variables,
    };
}
