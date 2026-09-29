#pragma once
#include <stdint.h>

typedef struct {
    uint32_t value;
} FunctionReferenceOperand;

uint32_t FunctionReferenceOperand_delta(FunctionReferenceOperand operand);
uint32_t FunctionReferenceOperand_functionIndex(FunctionReferenceOperand operand);