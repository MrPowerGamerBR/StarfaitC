#include "functionreferenceoperand.h"

uint32_t FunctionReferenceOperand_delta(FunctionReferenceOperand operand) {
    return (operand.value << 4) >> 4; // low 28 bits
}

uint32_t FunctionReferenceOperand_functionIndex(FunctionReferenceOperand operand) {
    return operand.value;
}