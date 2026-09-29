#pragma once
#include <stdint.h>

#include "chunks/code_chunk.h"
#include "chunks/func_chunk.h"
#include "chunks/gen8_chunk.h"
#include "chunks/path_chunk.h"
#include "chunks/scpt_chunk.h"
#include "chunks/strg_chunk.h"
#include "chunks/vari_chunk.h"

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