#include "vm.h"

#include <stdio.h>
#include <stdlib.h>

#include "functionreferenceoperand.h"
#include "op.h"
#include "opword.h"
#include "instructiondatatype.h"
#include "utils.h"
#include "rvalue.h"

void handlePush(StarfaitVM* vm, StarfaitByteBuffer* buffer, uint32_t type1) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        case DATA_TYPE_DOUBLE:
            abort();
            break;
        case DATA_TYPE_FLOAT:
            abort();
            break;
        case DATA_TYPE_INT32:
            abort();
            break;
        case DATA_TYPE_INT64:
            abort();
            break;
        case DATA_TYPE_BOOLEAN:
            abort();
            break;
        case DATA_TYPE_VARIABLE:
            abort();
            break;
        case DATA_TYPE_STRING:
            uint32_t stringIndex = StarfaitByteBuffer_readUint32LE(buffer);
            VMStack_push(&vm->stack, RValue_createReferencedString(vm->wad->strg.strings[stringIndex]->string));
            break;
        case DATA_TYPE_INT16:
            abort();
            break;
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

    if (functionIndex == 0) {
        printf("I'm running funcIndex 0!\n");
        Function function = vm->wad->func.functions[functionIndex];
        printf("Name is %s\n", STRG_getString(&vm->wad->strg, function.name));

        char* output = RValue_toString(arguments[0]);

        printf("Game: %s\n", output);

        free(output);

        VMStack_push(&vm->stack, RValue_createUndefined());
    } else {
        abort();
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
            StarfaitByteBuffer_writeUint32LE(&buffer, 0); // For functions we don't need to "save" the nibble (w00t!)

            // And that's all that there's to it!
            nextDelta = FunctionReferenceOperand_delta(operand);
        }
    }
}

StarfaitVM* StarfaitVM_create(GameWAD* wad) {
    StarfaitVM* vm = calloc(1, sizeof(StarfaitVM));
    vm->wad = wad;
    remapReferences(vm);

    return vm;
}

void StarfaitVM_executeBytecodeInstructions(StarfaitVM* vm, StarfaitByteBuffer* buffer) {
    while (StarfaitByteBuffer_hasRemaining(buffer)) {
        size_t start = buffer->position;

        OpWord word = {.value = StarfaitByteBuffer_readUint32LE(buffer)};
        uint32_t opcode = OpWord_opcode(word);
        uint16_t type1 = OpWord_type1(word);
        uint16_t type2 = OpWord_type2(word);
        uint16_t extra = OpWord_extra(word);

        printf("%d %s %x %x %x %x\n", start, Op_getOpcodeName(opcode), opcode, OpWord_type1(word), OpWord_type2(word), OpWord_extra(word));

        switch (opcode) {
            case OP_PUSH: {
                handlePush(vm, buffer, type1);
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
        }
    }
}
