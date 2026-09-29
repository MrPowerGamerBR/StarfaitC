#pragma once
#include <stdint.h>

typedef struct {
    uint32_t value;
} VariableReferenceOperand;

int32_t VariableReferenceOperand_delta(VariableReferenceOperand operand);

int32_t VariableReferenceOperand_variableIndex(VariableReferenceOperand operand);
int32_t VariableReferenceOperand_referenceFlags(VariableReferenceOperand operand);

bool VariableReferenceOperand_hasArrayIndex(VariableReferenceOperand operand);

bool VariableReferenceOperand_hasInstanceIdOnStack(VariableReferenceOperand operand);

bool VariableReferenceOperand_isRoomInstanceId(VariableReferenceOperand operand);