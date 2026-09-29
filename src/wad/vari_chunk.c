#include "gen8_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "../charutils.h"
#include "../utils.h"
#include "variable.h"
#include "vari_chunk.h"

#include "arraylist_variable.h"

constexpr uint32_t ENTRY_SIZE = 20;

VARIChunk VARIChunk_parse(StarfaitByteBuffer* buffer, size_t chunkSize) {
    uint32_t start = buffer->position;
    uint32_t globalVariables = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t instanceVariables = StarfaitByteBuffer_readUint32LE(buffer); // Somehow it seems that modern GM games always have globalVariables == instanceVariables
    uint32_t localVariables = StarfaitByteBuffer_readUint32LE(buffer);

    // The WAD doesn't give us the entry count (fun!)
    uint32_t remaining = ((start + chunkSize) - buffer->position);
    uint32_t entryCount = remaining / ENTRY_SIZE;

    VariableArrayList* variables = VariableArrayList_create(entryCount);

    repeat(entryCount, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        uint32_t instanceType = StarfaitByteBuffer_readUint32LE(buffer);
        int32_t varId = StarfaitByteBuffer_readInt32LE(buffer);
        uint32_t occurrenceCount = StarfaitByteBuffer_readUint32LE(buffer);
        int32_t firstAddress = StarfaitByteBuffer_readInt32LE(buffer);

        VariableArrayList_add(
            variables,
            (Variable){
                .name = name,
                .instanceType = instanceType,
                .varId = varId,
                .occurrenceCount = occurrenceCount,
                .firstAddress = firstAddress
            }
        );
    }

    return (VARIChunk){
        .globalVariables = globalVariables,
        .instanceVariables = instanceVariables,
        .localVariables = localVariables,
        .variables = variables,
    };
}
