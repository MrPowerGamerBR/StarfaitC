#include "opword.h"

Op OpWord_opcode(OpWord opWord) {
    return (opWord.value >> 24) & 0xFF;
}

uint32_t OpWord_type1(OpWord opWord) {
    return (opWord.value >> 16) & 0xF;
}

uint32_t OpWord_type2(OpWord opWord) {
    return (opWord.value >> 20) & 0xF;
}

int16_t OpWord_extra(OpWord opWord) {
    return (int16_t) opWord.value;
}

CmpOp OpWord_comparisonFunction(OpWord opWord) {
    return (opWord.value >> 8) & 0xF;
}

int32_t OpWord_branchOffset(OpWord opWord) {
    return (opWord.value << 9) >> 7;
}