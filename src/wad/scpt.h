#pragma once
#include "script.h"
#include "../starfaitbytebuffer.h"

typedef struct {
    size_t scriptCount;
    Script* scripts;
} SCPT;

SCPT SCPT_parse(StarfaitByteBuffer* buffer);