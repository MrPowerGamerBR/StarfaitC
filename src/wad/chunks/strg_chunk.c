#include "strg_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../starfaitbytebuffer.h"
#include "../../utils.h"

STRGChunk STRGChunk_parse(StarfaitByteBuffer* buffer) {
    size_t addressCount;
    uint32_t* addresses;
    StarfaitByteBuffer_readAddresses(buffer, &addressCount, &addresses);

    StringWrapper** strings = malloc(addressCount * sizeof(StringWrapper*));

    repeat(addressCount, x) {
        uint32_t address = addresses[x];
        uint32_t start = buffer->position;

        printf("address: %d\n", address);
        StarfaitByteBuffer_jumpTo(buffer, address);
        uint32_t length = StarfaitByteBuffer_readUint32LE(buffer);
        char* string = StarfaitByteBuffer_readChars(buffer, length);

        printf("string: %s\n", string);

        StringWrapper* wrapper = calloc(1, sizeof(StringWrapper));
        // The address includes the size of the string, however anywhere else in the WAD it references the string without the size
        wrapper->address = address + 4;
        wrapper->string = string;

        strings[x] = wrapper;

        StarfaitByteBuffer_jumpTo(buffer, start);
    }

    free(addresses);

    return (STRGChunk) {
        .stringCount = addressCount,
        .strings = strings,
    };
}

char* STRGChunk_getString(STRGChunk* strg, StringPointer pointer) {
    // TODO: please please please use a HashMap later
    repeat(strg->stringCount, i) {
        StringWrapper* wrapper = strg->strings[i];

        if (wrapper->address == pointer.value) {
            return wrapper->string;
        }
    }

    abort();
}