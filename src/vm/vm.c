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

void handlePushImmediate(StarfaitVM* vm, uint16_t extra) {
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

void handleCall(StarfaitVM* vm, StarfaitByteBuffer* buffer, uint32_t extra) {
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
        uint16_t extra = OpWord_extra(word);

        printf("%lu %s %x %x %x %x\n", start, Op_getOpcodeName(opcode), opcode, OpWord_type1(word), OpWord_type2(word), OpWord_extra(word));

        switch (opcode) {
            case OP_PUSH: {
                handlePush(vm, buffer, type1);
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
