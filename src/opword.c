#include "opword.h"

uint8_t OpWord_opcode(OpWord opWord) {
    return (opWord.value >> 24) & 0xFF;
}

uint32_t OpWord_type1(OpWord opWord) {
    return (opWord.value >> 16) & 0xF;
}

uint32_t OpWord_type2(OpWord opWord) {
    return (opWord.value >> 20) & 0xF;
}

uint16_t OpWord_extra(OpWord opWord) {
    return (uint16_t) opWord.value;
}

uint32_t OpWord_comparisonFunction(OpWord opWord) {
    return (opWord.value >> 8) & 0xF;
}

uint32_t OpWord_branchOffset(OpWord opWord) {
    return (opWord.value << 9) >> 7;
}