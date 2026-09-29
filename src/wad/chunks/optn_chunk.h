#pragma once
#include "../../starfaitbytebuffer.h"
#include "arraylist_optnconstant.h"
#include "../optnflags.h"

typedef struct {
    OptnFlags flags;
    int32_t scale;
    int32_t colorDepth;
    int32_t windowColor;
    int32_t resolution;
    int32_t frequency;
    int32_t vertexSync;
    int32_t priority;
    int32_t loadImage;
    int32_t loadAlpha;
    OptnConstantArrayList* constants;
} OPTNChunk;

OPTNChunk OPTNChunk_parse(StarfaitByteBuffer* buffer);