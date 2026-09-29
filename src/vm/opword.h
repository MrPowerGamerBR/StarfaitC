#pragma once
#include <stdint.h>

#include "op.h"

typedef struct {
    uint32_t value;
} OpWord;

// Where's type 4? /j https://en.wikipedia.org/wiki/R4:_Ridge_Racer_Type_4

Opcode OpWord_opcode(OpWord opWord);
uint32_t OpWord_type1(OpWord opWord);
uint32_t OpWord_type2(OpWord opWord);
int16_t OpWord_extra(OpWord opWord);
uint32_t OpWord_comparisonFunction(OpWord opWord);
uint32_t OpWord_branchOffset(OpWord opWord);