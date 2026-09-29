#include "func_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../starfaitbytebuffer.h"
#include "../../utils.h"

FUNCChunk FUNCChunk_parse(StarfaitByteBuffer* buffer) {
    int32_t count = StarfaitByteBuffer_readInt32LE(buffer);
    FunctionArrayList* functions = FunctionArrayList_create(count);

    repeat(count, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        int32_t occurrenceCount = StarfaitByteBuffer_readInt32LE(buffer);
        int32_t firstAddress = StarfaitByteBuffer_readInt32LE(buffer);

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
