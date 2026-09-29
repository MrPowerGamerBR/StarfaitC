#include "opword.h"

#include <stdio.h>

Op OpWord_opcode(OpWord opWord) {
    return (int32_t) ((opWord.value >> 24) & 0xFF);
}

int32_t OpWord_type1(OpWord opWord) {
    return (int32_t) ((opWord.value >> 16) & 0xF);
}

int32_t OpWord_type2(OpWord opWord) {
    return (int32_t) ((opWord.value >> 20) & 0xF);
}

int16_t OpWord_extra(OpWord opWord) {
    return (int16_t) opWord.value;
}

CmpOp OpWord_comparisonFunction(OpWord opWord) {
    return (int32_t) ((opWord.value >> 8) & 0xF);
}

int32_t OpWord_branchOffset(OpWord opWord) {
    // The branch offset is 23 bits, but then we also need to get the first bit of the 23 bits to know if it is positive or negative
    int32_t low23Bits = opWord.value & 0x7FFFFF;
    return (int32_t) ((low23Bits ^ 0x400000) - 0x400000) * 4; // Multiplied by four because this is in instructions, not bytes
}