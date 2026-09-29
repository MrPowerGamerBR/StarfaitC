#include "functionreferenceoperand.h"

int32_t FunctionReferenceOperand_delta(FunctionReferenceOperand operand) {
    return (int32_t) ((operand.value << 4) >> 4); // low 28 bits
}

int32_t FunctionReferenceOperand_functionIndex(FunctionReferenceOperand operand) {
    return (int32_t) operand.value;
}