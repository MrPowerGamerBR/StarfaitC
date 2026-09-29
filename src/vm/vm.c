#include "vm.h"
#include "vm_stack.h"

#include <stdio.h>
#include <stdlib.h>

#include "functionreferenceoperand.h"
#include "op.h"
#include "opword.h"
#include "instructiondatatype.h"
#include "../utils.h"
#include "rvalue.h"
#include "variablereferenceoperand.h"
#include "variablescope.h"
#include "vm_builtins.h"
#include "../charutils.h"

void handlePush(StarfaitVM* vm, StarfaitByteBuffer* buffer, uint32_t type1) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_VARIABLE: TODO();
        case DATA_TYPE_STRING:
            uint32_t stringIndex = StarfaitByteBuffer_readUint32LE(buffer);
            VMStack_push(&vm->stack, RValue_createReferencedString(vm->wad->strg.strings[stringIndex]->string));
            break;
        case DATA_TYPE_INT16: TODO();
    }
}

void handlePushLocal(StarfaitVM* vm, StarfaitByteBuffer* buffer, uint32_t type1, int32_t extra) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_VARIABLE: {
            VariableReferenceOperand operand = (VariableReferenceOperand) {.value = StarfaitByteBuffer_readUint32LE(buffer)};

            int32_t arrayIndex = VariableReferenceOperand_hasArrayIndex(operand) ? VMStack_pop(&vm->stack).value.int32 : -1;
            int32_t instanceId = VariableReferenceOperand_hasInstanceIdOnStack(operand) ? VMStack_pop(&vm->stack).value.int32 : extra;

            RValue localVariable = VariableContainer_getVariable(&vm->callFrame->container, VariableReferenceOperand_variableIndex(operand));
            VMStack_push(&vm->stack, localVariable);
            break;
        };
        case DATA_TYPE_STRING: TODO();
        case DATA_TYPE_INT16: TODO();
    }
}


void handlePushImmediate(StarfaitVM* vm, int16_t extra) {
    VMStack_push(&vm->stack, RValue_createInt32(extra));
}

void handleConv(StarfaitVM* vm, uint16_t type1, uint16_t type2) {
    // type1 is the source type, type2 is the destination type
    InstructionDataType sourceType = InstructionDataType_byId(type1);
    InstructionDataType destinationType = InstructionDataType_byId(type2);
    RValue pop = VMStack_pop(&vm->stack);

    switch (destinationType) {
        // no-op, this is only useful if some day we decide to go with non-tagged RValues (that is, using two arrays, one for native values and another for RValues)
        // Because in that case, we would need to convert the RValue to the native type
        case DATA_TYPE_VARIABLE:
            VMStack_push(&vm->stack, pop);
            return;
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_STRING: TODO();
        case DATA_TYPE_INT16: TODO();
    }
}

void handleCall(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t extra) {
    FunctionReferenceOperand operand = (FunctionReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};
    uint32_t functionIndex = FunctionReferenceOperand_functionIndex(operand);

    // extra = parameter count
    RValue* arguments = calloc(extra, sizeof(RValue));
    repeat(extra, i) {
        arguments[i] = VMStack_pop(&vm->stack);
    }

    printf("Function Index is %d\n", functionIndex);

    Function function = vm->wad->func.functions[functionIndex];
    char* functionName = STRG_getString(&vm->wad->strg, function.name);

    // TODO: This is BAD, we NEED to use HashMaps for this later
    repeat(vm->builtinFunctionsArrayList->size, i) {
        BuiltinFunction builtinFunction = vm->builtinFunctionsArrayList->elements[i];
        if (CharUtils_charEquals(builtinFunction.name, functionName)) {
            RValue result = builtinFunction.builtinFunction(vm, extra, arguments);
            VMStack_push(&vm->stack, result);
            return;
        }
    }

    abort();
}

void handlePop(StarfaitVM* vm, StarfaitByteBuffer* buffer, uint16_t type1, int16_t extra) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_VARIABLE: {
            VariableReferenceOperand operand = (VariableReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};

            uint32_t varId = VariableReferenceOperand_variableIndex(operand);
            int32_t arrayIndex = VariableReferenceOperand_hasArrayIndex(operand) ? VMStack_pop(&vm->stack).value.int32 : -1;
            int32_t instanceId = VariableReferenceOperand_hasInstanceIdOnStack(operand) ? VMStack_pop(&vm->stack).value.int32 : extra;

            RValue poppedValue = VMStack_pop(&vm->stack);

            if (0 > instanceId) {
                VariableScope scope = VariableScope_byId(instanceId);

                printf("Write variable %d\n", varId);

                switch (scope) {
                    case VARIABLE_SCOPE_SELF: TODO();
                    case VARIABLE_SCOPE_OTHER: TODO();
                    case VARIABLE_SCOPE_GLOBAL: TODO();
                    case VARIABLE_SCOPE_LOCAL: {
                        VariableContainer_setVariable(&vm->callFrame->container, varId, poppedValue);
                    }
                }
            }
            break;
        }
        case DATA_TYPE_STRING: TODO();
        case DATA_TYPE_INT16: TODO();
    }
}


void handlePopz(StarfaitVM* vm) {
    VMStack_pop(&vm->stack);
}

/**
 * By default, each variable and function reference in the code references a delta offset on the `extra` field to the next reference of the variable.
 *
 * The remapper walks through all VARI variables and remaps the delta offsets to be VARI indices.
 */
void remapReferences(StarfaitVM* vm) {
    // For now only functions will be remapped
    StarfaitByteBuffer buffer = StarfaitByteBuffer_create(vm->wad->code.bytecode, vm->wad->code.bytecodeSize);

    repeat(vm->wad->func.functionCount, i) {
        Function function = vm->wad->func.functions[i];
        // This points to the INSTRUCTION address, NOT the operand address, which is why we do +4
        StarfaitByteBuffer_jumpTo(&buffer, (function.firstAddress - vm->wad->code.postAddressPosition) + 4);

        uint8_t nextDelta = 0;

        repeat(function.occurenceCount, j) {
            printf("Processing %d\n", j);
            StarfaitByteBuffer_skip(&buffer, nextDelta);

            FunctionReferenceOperand operand = {.value = StarfaitByteBuffer_readUint32LE(&buffer)};

            // We need to rewind because we want to rewrite the operand
            StarfaitByteBuffer_rewind(&buffer, 4);

            // Write the new function index!
            StarfaitByteBuffer_writeUint32LE(&buffer, i); // For functions we don't need to "save" the nibble (w00t!)

            // And that's all that there's to it!
            nextDelta = FunctionReferenceOperand_delta(operand);
        }
    }
}

StarfaitVM* StarfaitVM_create(GameWAD* wad) {
    StarfaitVM* vm = calloc(1, sizeof(StarfaitVM));
    BuiltinFunctionArrayList* builtinFunctionsArrayList = BuiltinFunctionArrayList_create(8);
    vm->builtinFunctionsArrayList = builtinFunctionsArrayList;
    vm->callFrame = calloc(1, sizeof(CallFrame));

    vm->wad = wad;
    VMBuiltins_registerBuiltins(vm);
    remapReferences(vm);

    return vm;
}

void StarfaitVM_executeBytecodeInstructions(StarfaitVM* vm, StarfaitByteBuffer* buffer) {
    while (StarfaitByteBuffer_hasRemaining(buffer)) {
        size_t start = buffer->position;

        OpWord word = {.value = StarfaitByteBuffer_readUint32LE(buffer)};
        Opcode opcode = OpWord_opcode(word);
        uint16_t type1 = OpWord_type1(word);
        uint16_t type2 = OpWord_type2(word);
        int16_t extra = OpWord_extra(word);

        // VM: [gml_Object_obj_test_Step_0] (8) [0x4565fff9] POP (type1: 00000005, type2: 00000006, extra: fffffff9) [stack=1 ["Howdy! Loritta is so cute!"]]
        printf("VM: (%lu) [%x] %s (type1: %08x, type2: %08x, extra: %08x) [stack=%d", start, word.value, Op_getOpcodeName(opcode), type1, type2, extra, vm->stack.top);
        printf(" ");
        bool isFirst = true;
        printf("[");
        repeat(vm->stack.top, i) {
            if (!isFirst)
                printf(", ");
            printf("%s", RValue_toString(vm->stack.stack[i]));
            isFirst = false;
        }
        printf("]");
        printf("]\n");

        switch (opcode) {
            case OP_PUSH: {
                handlePush(vm, buffer, type1);
                break;
            }
            case OP_PUSH_LOCAL: {
                handlePushLocal(vm, buffer, type1, extra);
                break;
            }
            case OP_PUSH_IMMEDIATE: {
                handlePushImmediate(vm, extra);
                break;
            }
            case OP_CALL: {
                handleCall(vm, buffer, extra);
                break;
            }
            case OP_POP: {
                handlePop(vm, buffer, type1, extra);
                break;
            }
            case OP_POPZ: {
                handlePopz(vm);
                break;
            }
            case OP_CONV: {
                handleConv(vm, type1, type2);
                break;
            }
            default:
                bye("I don't know how to handle opcode 0x%02X (%s)!", opcode, Op_getOpcodeName(opcode));
        }
    }
}
