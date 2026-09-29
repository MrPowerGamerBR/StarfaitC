#include "vm_stack.h"

void VMStack_push(VMStack* stack, RValue value) {
    require(stack->top != MAX_STACK_SIZE, "VM stack overflow!");
    stack->stack[stack->top++] = value;
}

RValue VMStack_pop(VMStack* stack) {
    require(stack->top != 0, "VM stack underflow!");
    return stack->stack[--stack->top];
}

RValue VMStack_peek(VMStack* stack) {
    require(stack->top != 0, "You can't peek an empty stack!");
    return stack->stack[stack->top - 1];
}

RValue VMStack_peekAt(VMStack* stack, uint32_t index) {
    require(stack->top != 0, "You can't peek an empty stack!");
    require(stack->top > index, "Trying to peek out of bounds!");
    return stack->stack[index];
}