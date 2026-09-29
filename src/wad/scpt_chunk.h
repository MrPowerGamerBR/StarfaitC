#pragma once
#include "script.h"
#include "../starfaitbytebuffer.h"
#include "arraylist_script.h"

typedef struct {
    ScriptArrayList* scripts;
} SCPTChunk;

SCPTChunk SCPTChunk_parse(StarfaitByteBuffer* buffer);