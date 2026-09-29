#include "strg.h"

#include <stdio.h>
#include <stdlib.h>

#include "scpt.h"
#include "script.h"

#include "arraylist_uint32.h"
#include "../starfaitbytebuffer.h"
#include "../utils.h"

SCPT SCPT_parse(StarfaitByteBuffer* buffer) {
    Uint32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);
    ScriptArrayList* scripts = ScriptArrayList_create(addresses->size);

    Uint32ArrayList_forEach(addresses, address, i) {
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

    auto scpt = (SCPT){
        .scripts = scripts
    };

    Uint32ArrayList_free(addresses);

    return scpt;
}
