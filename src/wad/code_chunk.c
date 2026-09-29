#include "strg_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "code_chunk.h"

#include "../starfaitbytebuffer.h"
#include "../utils.h"

CODEChunk CODEChunk_parse(StarfaitByteBuffer* buffer) {
    Uint32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);
    size_t postAddressPosition = buffer->position;
    CodeEntryArrayList* codeEntries = CodeEntryArrayList_create(addresses->size);

    // This may happen if it is a YYC game OR if it is just a game without any code
    if (addresses->size == 0) {
        free(addresses);
        return (CODEChunk){
            .codeEntries = codeEntries,
            .postAddressPosition = postAddressPosition,
            .bytecode = nullptr
        };
    }

    uint32_t minAddressTarget = Utils_minFromArray(addresses->elements, addresses->size);

    // We want to read ALL the bytecode at once!
    size_t bytecodeSize = minAddressTarget - postAddressPosition;
    uint8_t* bytecode = StarfaitByteBuffer_readBytes(buffer, bytecodeSize);

    repeat(addresses->size, i) {
        uint32_t address = addresses->elements[i];
        StarfaitByteBuffer_jumpTo(buffer, address);

        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        uint32_t length = StarfaitByteBuffer_readUint32LE(buffer);
        uint16_t localsCount = StarfaitByteBuffer_readUint16LE(buffer);
        uint16_t argumentsCount = StarfaitByteBuffer_readUint16LE(buffer); // TODO: Is this even used?
        size_t bytecodeRelativeOffsetFieldPosition = buffer->position;
        uint32_t bytecodeRelativeOffset = StarfaitByteBuffer_readUint32LE(buffer);
        uint32_t offset = StarfaitByteBuffer_readUint32LE(buffer);

        CodeEntryArrayList_add(
            codeEntries,
            (CodeEntry){
                .name = name,
                .length = length,
                .localsCount = localsCount,
                .argumentsCount = argumentsCount,
                .bytecodeRelativeOffsetFieldPosition = bytecodeRelativeOffsetFieldPosition,
                .bytecodeRelativeOffset = bytecodeRelativeOffset,
                .offset = offset
            }
        );
    }

    CODEChunk code = (CODEChunk){
        .codeEntries = codeEntries,
        .postAddressPosition = postAddressPosition,
        .bytecodeSize = bytecodeSize,
        .bytecode = bytecode
    };

    Uint32ArrayList_free(addresses);
    return code;
}
