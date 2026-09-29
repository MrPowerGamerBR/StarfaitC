#include "variablereferenceoperand.h"

uint32_t VariableReferenceOperand_delta(VariableReferenceOperand operand) {
    return (operand.value << 4) >> 4; // low 28 bits
}

int32_t VariableReferenceOperand_variableIndex(VariableReferenceOperand operand) {
    return VariableReferenceOperand_delta(operand);
}

uint32_t VariableReferenceOperand_referenceFlags(VariableReferenceOperand operand) {
    return operand.value >> 28;
}

bool VariableReferenceOperand_hasArrayIndex(VariableReferenceOperand operand) {
    return (operand.value & 0x80000000u) == 0;
}

bool VariableReferenceOperand_hasInstanceIdOnStack(VariableReferenceOperand operand) {
    return (operand.value & 0x20000000) == 0;
}

bool VariableReferenceOperand_isRoomInstanceId(VariableReferenceOperand operand) {
    return (operand.value & 0x40000000) != 0;
}