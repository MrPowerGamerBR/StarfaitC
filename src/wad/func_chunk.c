#include "func_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "../starfaitbytebuffer.h"
#include "../utils.h"

FUNCChunk FUNCChunk_parse(StarfaitByteBuffer* buffer) {
    size_t count = StarfaitByteBuffer_readUint32LE(buffer);
    FunctionArrayList* functions = FunctionArrayList_create(count);

    repeat(count, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        uint32_t occurrenceCount = StarfaitByteBuffer_readUint32LE(buffer);
        uint32_t firstAddress = StarfaitByteBuffer_readUint32LE(buffer);

        FunctionArrayList_add(
            functions,
            (Function){
                .name = name,
                .occurrenceCount = occurrenceCount,
                .firstAddress = firstAddress,
            }
        );
    }

    return (FUNCChunk){
        .functions = functions,
    };
}
