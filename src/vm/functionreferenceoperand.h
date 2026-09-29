#pragma once
#include <stdint.h>

typedef struct {
    uint32_t value;
} FunctionReferenceOperand;

int32_t FunctionReferenceOperand_delta(FunctionReferenceOperand operand);
int32_t FunctionReferenceOperand_functionIndex(FunctionReferenceOperand operand);