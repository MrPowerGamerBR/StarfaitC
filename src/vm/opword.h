#pragma once
#include <stdint.h>

#include "op.h"
#include "cmpop.h"

typedef struct {
    uint32_t value;
} OpWord;

// Where's type 4? /j https://en.wikipedia.org/wiki/R4:_Ridge_Racer_Type_4

Op OpWord_opcode(OpWord opWord);
int32_t OpWord_type1(OpWord opWord);
int32_t OpWord_type2(OpWord opWord);
int16_t OpWord_extra(OpWord opWord);
CmpOp OpWord_comparisonFunction(OpWord opWord);
int32_t OpWord_branchOffset(OpWord opWord);