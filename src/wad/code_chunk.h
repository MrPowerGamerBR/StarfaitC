#pragma once
#include "arraylist_codeentry.h"
#include "../starfaitbytebuffer.h"

typedef struct {
    CodeEntryArrayList* codeEntries;
    uint32_t postAddressPosition;
    size_t bytecodeSize;
    uint8_t* bytecode;
} CODEChunk;

CODEChunk CODEChunk_parse(StarfaitByteBuffer* buffer);