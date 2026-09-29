#include "wad.h"

#include <stdio.h>
#include <stdlib.h>

#include "code_chunk.h"
#include "gen8_chunk.h"
#include "scpt_chunk.h"
#include "strg_chunk.h"
#include "vari_chunk.h"
#include "../charutils.h"

GameWAD GameWAD_parse(StarfaitByteBuffer* buffer) {
    char* header = StarfaitByteBuffer_readChars(buffer, 4);

    // TODO: require
    if (!CharUtils_charEquals(header, "FORM")) abort();

    printf("Header is %s\n", header);

    uint32_t count = StarfaitByteBuffer_readUint32LE(buffer);

    printf("Size is %d\n", count);

    GEN8Chunk gen8;
    STRGChunk strg;
    VARIChunk vari;
    CODEChunk code;
    FUNCChunk func;
    SCPTChunk scpt;

    while (StarfaitByteBuffer_hasRemaining(buffer)) {
        char* chunkName = StarfaitByteBuffer_readChars(buffer, 4);
        uint32_t chunkSize = StarfaitByteBuffer_readUint32LE(buffer);
        size_t currentPosition = buffer->position;

        printf("Chunk is %s (size: %d)\n", chunkName, chunkSize);

        if (CharUtils_charEquals(chunkName, "GEN8")) gen8 = GEN8Chunk_parse(buffer);
        if (CharUtils_charEquals(chunkName, "STRG")) strg = STRGChunk_parse(buffer);
        if (CharUtils_charEquals(chunkName, "VARI")) vari = VARIChunk_parse(buffer, chunkSize);
        if (CharUtils_charEquals(chunkName, "CODE")) code = CODEChunk_parse(buffer);
        if (CharUtils_charEquals(chunkName, "FUNC")) func = FUNCChunk_parse(buffer);
        if (CharUtils_charEquals(chunkName, "SCPT")) scpt = SCPTChunk_parse(buffer);

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
