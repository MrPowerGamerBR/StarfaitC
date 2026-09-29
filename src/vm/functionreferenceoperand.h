#pragma once
#include <stdint.h>

typedef struct {
    uint32_t value;
} FunctionReferenceOperand;

// Where's type 4? /j https://en.wikipedia.org/wiki/R4:_Ridge_Racer_Type_4

uint32_t FunctionReferenceOperand_delta(FunctionReferenceOperand operand);
uint32_t FunctionReferenceOperand_functionIndex(FunctionReferenceOperand operand);