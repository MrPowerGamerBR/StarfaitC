#include "wad.h"

#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "gen8.h"
#include "scpt.h"
#include "strg.h"
#include "vari.h"
#include "../charutils.h"

GameWAD GameWAD_parse(StarfaitByteBuffer* buffer) {
    char* header = StarfaitByteBuffer_readChars(buffer, 4);

    // TODO: require
    if (!CharUtils_charEquals(header, "FORM")) abort();

    printf("Header is %s\n", header);

    uint32_t count = StarfaitByteBuffer_readUint32LE(buffer);

    printf("Size is %d\n", count);

    GEN8 gen8;
    STRG strg;
    VARI vari;
    CODE code;
    FUNC func;
    SCPT scpt;

    while (StarfaitByteBuffer_hasRemaining(buffer)) {
        char* chunkName = StarfaitByteBuffer_readChars(buffer, 4);
        uint32_t chunkSize = StarfaitByteBuffer_readUint32LE(buffer);
        size_t currentPosition = buffer->position;

        printf("Chunk is %s (size: %d)\n", chunkName, chunkSize);

        if (CharUtils_charEquals(chunkName, "GEN8")) gen8 = GEN8_parse(buffer);
        if (CharUtils_charEquals(chunkName, "STRG")) strg = STRG_parse(buffer);
        if (CharUtils_charEquals(chunkName, "VARI")) vari = VARI_parse(buffer, chunkSize);
        if (CharUtils_charEquals(chunkName, "CODE")) code = CODE_parse(buffer);
        if (CharUtils_charEquals(chunkName, "FUNC")) func = FUNC_parse(buffer);
        if (CharUtils_charEquals(chunkName, "SCPT")) scpt = SCPT_parse(buffer);

        StarfaitByteBuffer_jumpTo(buffer, currentPosition + chunkSize);
    }

    return (GameWAD) {
        .gen8 = gen8,
        .strg = strg,
        .vari = vari,
        .code = code,
        .func = func,
        .scpt = scpt
    };
}
