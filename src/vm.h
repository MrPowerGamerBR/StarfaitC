#pragma once
#include "wad/code.h"
#include "wad/wad.h"

#define ARRAY_LIST_NAME Int32ArrayList
#define ARRAY_LIST_TYPE uint32_t
#include "arraylist.h"
#undef ARRAY_LIST_NAME

struct {
    GameWAD* wad;
} typedef StarfaitVM;

StarfaitVM* StarfaitVM_create(GameWAD* wad);

void StarfaitVM_executeBytecodeInstructions(StarfaitVM* vm, StarfaitByteBuffer* buffer);