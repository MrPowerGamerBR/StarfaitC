#include "strg.h"

#include <stdio.h>
#include <stdlib.h>

#include "scpt.h"
#include "script.h"

#include "../starfaitbytebuffer.h"
#include "../utils.h"

SCPT SCPT_parse(StarfaitByteBuffer* buffer) {
    Uint32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);

    Script* script = calloc(addresses->size, sizeof(Script));

    repeat(addresses->size, i) {
        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        int32_t codeIndex = StarfaitByteBuffer_readInt32LE(buffer);

        script[i] = (Script) {
            .name = name,
            .codeIndex = codeIndex
        };
    }

    auto scpt = (SCPT) {
        .scriptCount = addresses->size,
        .scripts = script,
    };

    Uint32ArrayList_free(addresses);

    return scpt;
}