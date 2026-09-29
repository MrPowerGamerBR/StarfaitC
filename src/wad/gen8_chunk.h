#pragma once
#include "../starfaitbytebuffer.h"

typedef struct {
    bool debugWad;
    uint8_t wadVersion;
    StringPointer gameTitle;
    StringPointer yoyoConfig;
    uint8_t maxObjectId;
    uint8_t maxTileId;
    uint32_t gameId;
    uint32_t roomOrderCount;
    uint32_t* roomOrder;
} GEN8Chunk;

GEN8Chunk GEN8Chunk_parse(StarfaitByteBuffer* buffer);