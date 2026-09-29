#include "gen8_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../charutils.h"
#include "../../utils.h"
#include "../variable.h"
#include "vari_chunk.h"

#include "arraylist_variable.h"

constexpr int32_t ENTRY_SIZE = 20;

VARIChunk VARIChunk_parse(StarfaitByteBuffer* buffer, int32_t chunkSize) {
    int32_t start = buffer->position;
    int32_t globalVariables = StarfaitByteBuffer_readInt32LE(buffer);
    int32_t instanceVariables = StarfaitByteBuffer_readInt32LE(buffer); // Somehow it seems that modern GM games always have globalVariables == instanceVariables
    int32_t localVariables = StarfaitByteBuffer_readInt32LE(buffer);

    // The WAD doesn't give us the entry count (fun!)
    int32_t remaining = ((start + chunkSize) - buffer->position);
    int32_t entryCount = remaining / ENTRY_SIZE;

    VariableArrayList* variables = VariableArrayList_create(entryCount);

    repeat(entryCount, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        int32_t instanceType = StarfaitByteBuffer_readInt32LE(buffer);
        int32_t varId = StarfaitByteBuffer_readInt32LE(buffer);
        int32_t occurrenceCount = StarfaitByteBuffer_readInt32LE(buffer);
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
