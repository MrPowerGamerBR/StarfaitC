#pragma once
#include "variable.h"
#include "../starfaitbytebuffer.h"

struct {
    StringPointer name;
    uint32_t length;
    uint32_t localsCount;
    uint32_t argumentsCount;
    uint32_t bytecodeRelativeOffsetFieldPosition;
    uint32_t bytecodeRelativeOffset;
    uint32_t offset;
} typedef CodeEntry;

struct {
    size_t codeEntryCount;
    CodeEntry* codeEntries;
    uint32_t postAddressPosition;
    size_t bytecodeSize;
    uint8_t* bytecode;
} typedef CODE;

CODE CODE_parse(StarfaitByteBuffer* buffer);