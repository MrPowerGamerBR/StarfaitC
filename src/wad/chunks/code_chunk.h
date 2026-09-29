#pragma once
#include "arraylist_codeentry.h"
#include "../../starfaitbytebuffer.h"

typedef struct {
    CodeEntryArrayList* codeEntries;
    int32_t postAddressPosition;
    int32_t bytecodeSize;
    uint8_t* bytecode;
} CODEChunk;

CODEChunk CODEChunk_parse(StarfaitByteBuffer* buffer);