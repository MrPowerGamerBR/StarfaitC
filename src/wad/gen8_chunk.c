#include "gen8_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"
#include "../charutils.h"

GEN8Chunk GEN8Chunk_parse(StarfaitByteBuffer* buffer) {
    bool debugWad = StarfaitByteBuffer_readUint8Boolean(buffer);
    uint8_t wadVersion = StarfaitByteBuffer_readUint8(buffer);
    StarfaitByteBuffer_skip(buffer, 2); // Padding
    StringPointer gameTitle = StarfaitByteBuffer_readStringPointer(buffer);
    StringPointer yoyoConfig = StarfaitByteBuffer_readStringPointer(buffer);
    uint32_t maxObjectId = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t maxTileId = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t gameId = StarfaitByteBuffer_readUint32LE(buffer);
    uint8_t* gameGuid = StarfaitByteBuffer_readBytes(buffer, 16);
    StringPointer gameProjectName = StarfaitByteBuffer_readStringPointer(buffer);
    uint32_t major = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t minor = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t release = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t build = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t initialApplicationWidth = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t initialApplicationHeight = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t initialScreenFlags = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t md5Crc = StarfaitByteBuffer_readUint32LE(buffer);
    uint8_t* md5 = StarfaitByteBuffer_readBytes(buffer, 16);
    uint64_t wadDateTime = StarfaitByteBuffer_readUint64LE(buffer);
    StringPointer gameDisplayName = StarfaitByteBuffer_readStringPointer(buffer);
    uint64_t licensedTargets = StarfaitByteBuffer_readUint64LE(buffer);
    uint64_t functionClassifications = StarfaitByteBuffer_readUint64LE(buffer);
    uint32_t steamAppId = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t debuggerServerPort = StarfaitByteBuffer_readUint32LE(buffer);
    uint32_t roomOrderCount = StarfaitByteBuffer_readUint32LE(buffer);

    uint32_t* roomOrder = malloc(sizeof(uint32_t) * roomOrderCount);

    repeat(roomOrderCount, x) {
        uint32_t roomId = StarfaitByteBuffer_readUint32LE(buffer);
        roomOrder[x] = roomId;
    }

    return (GEN8Chunk) {
        .debugWad = debugWad,
        .wadVersion = wadVersion,
        .gameTitle = gameTitle,
        .yoyoConfig = yoyoConfig,
        .maxObjectId = maxObjectId,
        .maxTileId = maxTileId,
        .gameId = gameId,
        .roomOrderCount = roomOrderCount,
        .roomOrder = roomOrder
    };
}
