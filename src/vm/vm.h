#pragma once
#include "../wad/code.h"
#include "../wad/wad.h"
#include "vm_stack.h"

typedef struct {
    GameWAD* wad;
    VMStack stack;
} StarfaitVM;

StarfaitVM* StarfaitVM_create(GameWAD* wad);

void StarfaitVM_executeBytecodeInstructions(StarfaitVM* vm, StarfaitByteBuffer* buffer);