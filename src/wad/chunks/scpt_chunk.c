#include "strg_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "scpt_chunk.h"
#include "../script.h"

#include "arraylist_uint32.h"
#include "../../starfaitbytebuffer.h"
#include "../../utils.h"

SCPTChunk SCPTChunk_parse(StarfaitByteBuffer* buffer) {
    Int32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);
    ScriptArrayList* scripts = ScriptArrayList_create(addresses->size);

    Int32ArrayList_forEach(addresses, address, i) {
        StarfaitByteBuffer_jumpTo(buffer, *address);

        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        int32_t codeIndex = StarfaitByteBuffer_readInt32LE(buffer);

        ScriptArrayList_add(
            scripts,
            (Script){
                .name = name,
                .codeIndex = codeIndex
            }
        );
    }

    auto scpt = (SCPTChunk){
        .scripts = scripts
    };

    Int32ArrayList_free(addresses);

    return scpt;
}
