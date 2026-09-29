#pragma once
#include <stddef.h>
#include <stdint.h>

#include "rvalue.h"

constexpr size_t MAX_STACK_SIZE = 1024;

typedef struct {
    uint32_t top;
    RValue stack[MAX_STACK_SIZE];
} VMStack;

void VMStack_push(VMStack* stack, RValue value);
RValue VMStack_pop(VMStack* stack);
RValue VMStack_peek(VMStack* stack);