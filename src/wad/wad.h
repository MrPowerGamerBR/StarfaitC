#pragma once
#include <stdint.h>

#include "code_chunk.h"
#include "func_chunk.h"
#include "gen8_chunk.h"
#include "path_chunk.h"
#include "scpt_chunk.h"
#include "strg_chunk.h"
#include "vari_chunk.h"

typedef struct {
    GEN8Chunk gen8;
    VARIChunk vari;
    STRGChunk strg;
    FUNCChunk func;
    CODEChunk code;
    SCPTChunk scpt;
    PATHChunk path;
} GameWAD;

GameWAD GameWAD_parse(StarfaitByteBuffer* buffer);