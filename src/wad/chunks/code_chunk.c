#include "strg_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "code_chunk.h"

#include "../../starfaitbytebuffer.h"
#include "../../utils.h"

CODEChunk CODEChunk_parse(StarfaitByteBuffer* buffer) {
    Int32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);
    int32_t postAddressPosition = buffer->position;
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

    int32_t minAddressTarget = Int32ArrayList_min(addresses);

    // We want to read ALL the bytecode at once!
    int32_t bytecodeSize = minAddressTarget - postAddressPosition;
    uint8_t* bytecode = StarfaitByteBuffer_readBytes(buffer, bytecodeSize);

    Int32ArrayList_forEach(addresses, _address, i) {
        int32_t address = *_address;
        StarfaitByteBuffer_jumpTo(buffer, address);

        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        int32_t length = StarfaitByteBuffer_readInt32LE(buffer);
        uint16_t localsCount = StarfaitByteBuffer_readUint16LE(buffer);
        uint16_t argumentsCount = StarfaitByteBuffer_readUint16LE(buffer); // TODO: Is this even used?
        int32_t bytecodeRelativeOffsetFieldPosition = buffer->position;
        int32_t bytecodeRelativeOffset = StarfaitByteBuffer_readInt32LE(buffer);
        int32_t offset = StarfaitByteBuffer_readInt32LE(buffer);

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

    Int32ArrayList_free(addresses);
    return code;
}
