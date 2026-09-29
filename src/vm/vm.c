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
#include "../starfaitstring.h"
#include "arraylist_string.h"

constexpr int32_t REGULAR_VARIABLES_BASE = 100'000;

CallFrame* StarfaitVM_getCurrentCallFrame(StarfaitVM* vm) {
    return CallFrameArrayList_last(vm->callFrameStack);
}

/**
 * Reads variables from the instanceId, based on the current context
 *
 * @param vm the StarfaitVM instance
 * @param varId the variable ID
 * @param arrayIndex the array index, -1 if there isn't any
 * @param instanceId the instance ID
 * @return the RValue (not copied)
 */
static RValue readVariableFromInstanceId(StarfaitVM* vm, int32_t varId, int32_t arrayIndex, int32_t instanceId) {
    // TECHNICALLY I'm pretty sure that not all push paths write to builtin vs regular variable
    // But to make everything consistent, we'll use the same path for everything
    VariableContainer* variableContainer = nullptr;

    if (0 > instanceId) {
        VariableScope scope = VariableScope_byId((int8_t) instanceId);

        switch (scope) {
            case VARIABLE_SCOPE_SELF: TODO();
            case VARIABLE_SCOPE_OTHER: TODO();
            case VARIABLE_SCOPE_GLOBAL: {
                variableContainer = &vm->global->container;
                break;
            }
            case VARIABLE_SCOPE_LOCAL: {
                variableContainer = &StarfaitVM_getCurrentCallFrame(vm)->container;
                break;
            }
        }
    } else TODO();

    RValue rvalue = VariableContainer_getVariable(variableContainer, varId);
    return rvalue;
}

/**
 * Prints all variables that a VariableContainer holds, useful for debugging!
 *
 * @param vm the StarfaitVM instance
 * @param container the VariableContainer that you want to see their variables
 */
static void printVariables(StarfaitVM* vm, VariableContainer* container) {
    // TODO: It would be cool if the HashMap itself had a "entries" similar to Java's HashMap
    repeat(container->variables->bucketsCount, bucket) {
        Int32RValueHashMapEntry* entry = container->variables->buckets[bucket];
        while (entry != nullptr) {
            printf("%s=%s\n", StarfaitString_toCharArrayView(StringArrayList_get(vm->regularVariableNames, entry->key - REGULAR_VARIABLES_BASE)), RValue_toString(entry->value));
            entry = entry->next;
        }
    }
}

static void handlePush(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1, int32_t extra) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        // While there's already PUSH_IMMEDIATE for INT16, it seems that GameMaker also does use handlePush with INT16 sometimes, in this case the value is the "extra" field
        case DATA_TYPE_INT16: {
            VMStack_push(&vm->stack, RValue_createInt32(extra));
            break;
        }
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: {
            VMStack_push(&vm->stack, RValue_createInt32(StarfaitByteBuffer_readInt32LE(buffer)));
            break;
        };
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_VARIABLE: {
            VariableReferenceOperand operand = (VariableReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};

            int32_t arrayIndex = VariableReferenceOperand_hasArrayIndex(operand) ? VMStack_pop(&vm->stack).value.int32 : -1;
            int32_t instanceId = VariableReferenceOperand_hasInstanceIdOnStack(operand) ? VMStack_pop(&vm->stack).value.int32 : extra;

            RValue variable = readVariableFromInstanceId(vm, VariableReferenceOperand_variableIndex(operand), arrayIndex, instanceId);
            VMStack_push(&vm->stack, RValue_createCopy(variable));
            break;
        };
        case DATA_TYPE_STRING:
            uint32_t stringIndex = StarfaitByteBuffer_readUint32LE(buffer);
            VMStack_push(&vm->stack, RValue_createStringFromCStringCopy(vm->wad->strg.strings[stringIndex]->string));
            break;
    }
}

static void handlePushLocal(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1, int32_t extra) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_VARIABLE: {
            VariableReferenceOperand operand = (VariableReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};

            int32_t arrayIndex = VariableReferenceOperand_hasArrayIndex(operand) ? VMStack_pop(&vm->stack).value.int32 : -1;
            int32_t instanceId = VariableReferenceOperand_hasInstanceIdOnStack(operand) ? VMStack_pop(&vm->stack).value.int32 : extra;

            RValue variable = readVariableFromInstanceId(vm, VariableReferenceOperand_variableIndex(operand), arrayIndex, instanceId);
            VMStack_push(&vm->stack, RValue_createCopy(variable));
            break;
        };
        case DATA_TYPE_STRING: TODO();
        case DATA_TYPE_INT16: TODO();
    }
}

static void handlePushBuiltin(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1) {
    // Pushes a builtin variable to the stack
    // handlePushBuiltin type1 is always "VARIABLE"
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    VariableReferenceOperand operand = (VariableReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};
    int32_t varId = VariableReferenceOperand_variableIndex(operand);

    printf("varID: %d\n", varId);

    Variable* variable = VariableArrayList_get(vm->wad->vari.variables, varId);
    printf("Variable is %s\n", STRGChunk_getString(&vm->wad->strg, variable->name));

    RValue result = vm->builtins->builtinVariablesArrayList->elements[0].builtinVariableReader(vm, -1);
    VMStack_push(&vm->stack, result);
}

static void handlePushImmediate(StarfaitVM* vm, int32_t extra) {
    VMStack_push(&vm->stack, RValue_createInt32(extra));
}

static void handleConv(StarfaitVM* vm, int32_t type1, int32_t type2) {
    // type1 is the source type, type2 is the destination type
    InstructionDataType sourceType = InstructionDataType_byId(type1);
    InstructionDataType destinationType = InstructionDataType_byId(type2);
    RValue pop = VMStack_pop(&vm->stack);

    switch (destinationType) {
        // no-op, this is only useful if some day we decide to go with non-tagged RValues (that is, using two arrays, one for native values and another for RValues)
        // Because in that case, we would need to convert the RValue to the native type
        case DATA_TYPE_VARIABLE: {
            VMStack_push(&vm->stack, pop);
            return;
        }
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: {
            VMStack_push(&vm->stack, RValue_createBoolean(RValue_getAsBoolean(pop)));
            return;
        }
        case DATA_TYPE_STRING: TODO();
        case DATA_TYPE_INT16: TODO();
    }
}

static void handleCall(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t extra) {
    FunctionReferenceOperand operand = (FunctionReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};
    int32_t functionIndex = FunctionReferenceOperand_functionIndex(operand);

    // extra = parameter count
    RValue* arguments = calloc((size_t) extra, sizeof(RValue));
    repeat(extra, i) {
        arguments[i] = VMStack_pop(&vm->stack);
    }

    printf("Function Index is %d\n", functionIndex);

    Function function = *FunctionArrayList_get(vm->wad->func.functions, functionIndex);
    char* functionName = STRGChunk_getString(&vm->wad->strg, function.name);

    // TODO: This is BAD, we NEED to use HashMaps for this later
    BuiltinFunctionArrayList_forEach(vm->builtins->builtinFunctionsArrayList, builtinFunction, i) {
        if (CharUtils_charEquals(builtinFunction->name, functionName)) {
            RValue result = builtinFunction->builtinFunction(vm, extra, arguments);
            VMStack_push(&vm->stack, result);

            // Free call arguments
            repeat(extra, j) {
                RValue_free(arguments[j]);
            }
            return;
        }
    }

    // This may be a script!
    // TODO: Maybe have a HashMap for this too?
    ScriptArrayList_forEach(vm->wad->scpt.scripts, script, i) {
        char* scriptName = STRGChunk_getString(&vm->wad->strg, script->name);

        if (CharUtils_charEquals(scriptName, functionName)) {
            CodeEntry* codeEntry = CodeEntryArrayList_get(vm->wad->code.codeEntries, script->codeIndex);

            RValue value = StarfaitVM_executeCode(vm, codeEntry, RValueArrayList_createFromCArray(arguments, extra));
            VMStack_push(&vm->stack, value);

            // Free call arguments
            repeat(extra, j) {
                RValue_free(arguments[j]);
            }
            return;
        }
    }

    abort();
}

static void handlePop(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1, int32_t extra) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);

    switch (type1DataType) {
        case DATA_TYPE_DOUBLE: TODO();
        case DATA_TYPE_FLOAT: TODO();
        case DATA_TYPE_INT32: TODO();
        case DATA_TYPE_INT64: TODO();
        case DATA_TYPE_BOOLEAN: TODO();
        case DATA_TYPE_VARIABLE: {
            VariableReferenceOperand operand = (VariableReferenceOperand){.value = StarfaitByteBuffer_readUint32LE(buffer)};

            int32_t varId = VariableReferenceOperand_variableIndex(operand);
            int32_t arrayIndex = VariableReferenceOperand_hasArrayIndex(operand) ? VMStack_pop(&vm->stack).value.int32 : -1;
            int32_t instanceId = VariableReferenceOperand_hasInstanceIdOnStack(operand) ? VMStack_pop(&vm->stack).value.int32 : extra;

            RValue poppedValue = VMStack_pop(&vm->stack);

            if (0 > instanceId) {
                VariableScope scope = VariableScope_byId(instanceId);

                printf("Write variable %d\n", varId);

                switch (scope) {
                    case VARIABLE_SCOPE_SELF: TODO();
                    case VARIABLE_SCOPE_OTHER: TODO();
                    case VARIABLE_SCOPE_GLOBAL: {
                        GlobalObject* callFrame = vm->global;
                        VariableContainer_setVariable(&callFrame->container, varId, poppedValue);
                        break;
                    }
                    case VARIABLE_SCOPE_LOCAL: {
                        // We don't need to copy the variable because we "steal" from the stack
                        CallFrame* callFrame = StarfaitVM_getCurrentCallFrame(vm);
                        VariableContainer_setVariable(&callFrame->container, varId, poppedValue);
                        break;
                    }
                }
            }
            break;
        }
        case DATA_TYPE_STRING: TODO();
        case DATA_TYPE_INT16: TODO();
    }
}

static void handlePopz(StarfaitVM* vm) {
    VMStack_pop(&vm->stack);
}

static void handleAdd(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1, int32_t type2) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);
    InstructionDataType type2DataType = InstructionDataType_byId(type2);

    // The YoYo Runner uses the type1/type2 data types to know how many bytes to read from the stack
    // Because we use tagged RValues, we don't need them for this
    RValue right = VMStack_pop(&vm->stack);
    RValue left = VMStack_pop(&vm->stack);

    if (left.type == RVALUE_DATA_TYPE_STRING || right.type == RVALUE_DATA_TYPE_STRING) {
        if (left.type != RVALUE_DATA_TYPE_STRING || right.type != RVALUE_DATA_TYPE_STRING) {
            bye("DoAdd :: Execution Error");
        }

        StarfaitString* concat = StarfaitString_concat(left.value.string, right.value.string);

        VMStack_push(&vm->stack, RValue_createStringFromCStringCopy(strdup(StarfaitString_toCharArrayView(concat))));

        StarfaitString_free(concat);
        return;
    }

    VMStack_push(&vm->stack, RValue_createReal(RValue_getAsReal(left) + RValue_getAsReal(right)));
}

static void handleSub(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1, int32_t type2) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);
    InstructionDataType type2DataType = InstructionDataType_byId(type2);

    // The YoYo Runner uses the type1/type2 data types to know how many bytes to read from the stack
    // Because we use tagged RValues, we don't need them for this
    RValue right = VMStack_pop(&vm->stack);
    RValue left = VMStack_pop(&vm->stack);
    VMStack_push(&vm->stack, RValue_createReal(RValue_getAsReal(left) - RValue_getAsReal(right)));
}

static void handleCmp(StarfaitVM* vm, StarfaitByteBuffer* buffer, CmpOp cmpOp, int32_t type1, int32_t type2) {
    InstructionDataType type1DataType = InstructionDataType_byId(type1);
    InstructionDataType type2DataType = InstructionDataType_byId(type2);

    // The YoYo Runner uses the type1/type2 data types to know how many bytes to read from the stack
    // Because we use tagged RValues, we don't need them for this
    RValue right = VMStack_pop(&vm->stack);
    RValue left = VMStack_pop(&vm->stack);

    double leftReal = RValue_getAsReal(left);
    double rightReal = RValue_getAsReal(right);

    switch (cmpOp) {
        case CMPOP_EQUAL: {
            VMStack_push(&vm->stack, RValue_createBoolean(leftReal == rightReal));
            break;
        }
        case CMPOP_NOT_EQUAL: {
            VMStack_push(&vm->stack, RValue_createBoolean(leftReal != rightReal));
            break;
        }
        case CMPOP_LESS_THAN: {
            VMStack_push(&vm->stack, RValue_createBoolean(leftReal < rightReal));
            break;
        }
        case CMPOP_LESS_THAN_OR_EQUAL: {
            VMStack_push(&vm->stack, RValue_createBoolean(leftReal <= rightReal));
            break;
        }
        case CMPOP_GREATER_THAN_OR_EQUAL: {
            VMStack_push(&vm->stack, RValue_createBoolean(leftReal >= rightReal));
            break;
        }
        case CMPOP_GREATER_THAN: {
            VMStack_push(&vm->stack, RValue_createBoolean(leftReal > rightReal));
            break;
        }
    }
}

static void handleB([[maybe_unused]] StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t branchOffset) {
    // Branches are in relation to the start of the instruction
    buffer->position += (branchOffset - 4);
}

static void handleBT(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t branchOffset) {
    bool result = RValue_getAsBoolean(VMStack_pop(&vm->stack));
    if (result) {
        // Branches are in relation to the start of the instruction
        buffer->position += (branchOffset - 4);
    }
}

static void handleBF(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t branchOffset) {
    bool result = RValue_getAsBoolean(VMStack_pop(&vm->stack));
    if (!result) {
        // Branches are in relation to the start of the instruction
        buffer->position += (branchOffset - 4);
    }
}

static void handleDup(StarfaitVM* vm, StarfaitByteBuffer* buffer, int32_t type1, int32_t extra) {
    // In the YoYo Runner, the type1 would've been used to figure out how many bytes to be copied from the stack
    // Because all of our types are tagged RValues, we don't need to rely on it (yay)
    // extra = how many additional elements will be copied from the stack, that is...
    // If the stack is [a, b], and extra is 1, the result will be [a, b, a, b]
    // TODO: TECHNICALLY while this does work with Undertale (I think?), newer GameMaker games do emit a pattern like "dup int 1" when the top of the stack is, in fact, not a integer
    //  See how Butterscotch handles handleDup for reference
    require(extra >= 0, "Negative extra value on the dup!");
    int32_t total = extra + 1;
    int32_t dupBottom = vm->stack.top - 1 - extra;
    repeat(total, i) {
        RValue target = VMStack_peekAt(&vm->stack, dupBottom + i);
        VMStack_push(&vm->stack, RValue_createCopy(target));
    }
}

/**
 * By default, each variable and function reference in the code references a delta offset on the `extra` field to the next reference of the variable.
 *
 * The remapper walks through all VARI variables and remaps the delta offsets to be VARI indices.
 */
static void remapReferences(StarfaitVM* vm) {
    // I'm not sure WHY the YoYo Runner does this!!
    StarfaitByteBuffer buffer = StarfaitByteBuffer_create(vm->wad->code.bytecode, vm->wad->code.bytecodeSize);
    VariableArrayList* allocatedBuiltinVariables = VariableArrayList_create(0);
    VariableArrayList* allocatedRegularVariables = VariableArrayList_create(0);

    VariableArrayList_forEach(vm->wad->vari.variables, variable, i) {
        if (variable->firstAddress != -1) {
            // Here's the thing:
            // For builtin variables, we store it sequentially, starting from 0
            // For regular variables, we store it sequentially, starting from REGULAR_VARIABLES_BASE
            // This way we can differentiate directly on the operand itself, and we can store the handlers densely
            int32_t variableHandlerId = 0;
            if (variable->varId == -6) {
                int32_t index = allocatedBuiltinVariables->size;
                VariableArrayList_add(allocatedBuiltinVariables, *variable);
                variableHandlerId = index;
            } else {
                int32_t index = allocatedRegularVariables->size;
                VariableArrayList_add(allocatedRegularVariables, *variable);
                variableHandlerId = (index + REGULAR_VARIABLES_BASE);
            }

            // This points to the INSTRUCTION address, NOT the operand address, which is why we do +4
            StarfaitByteBuffer_jumpTo(&buffer, (variable->firstAddress - vm->wad->code.postAddressPosition) + 4);

            int32_t nextDelta = 0;

            repeat(variable->occurrenceCount, j) {
                printf("Processing %d (%s) with variable handler ID %d (delta is %d)\n", j, STRGChunk_getString(&vm->wad->strg, variable->name), variableHandlerId, nextDelta);
                StarfaitByteBuffer_skip(&buffer, nextDelta);

                VariableReferenceOperand operand = {.value = StarfaitByteBuffer_readUint32LE(&buffer)};
                nextDelta = VariableReferenceOperand_delta(operand);

                // We need to rewind because we want to rewrite the operand
                StarfaitByteBuffer_rewind(&buffer, 4);

                // Write the new variable index!
                // First value: hasArrayIndex
                // Second value: The rest:tm:
                StarfaitByteBuffer_writeUint32LE(&buffer, (operand.value & 0xF0000000) | (variableHandlerId & 0x0FFFFFFF));

                // We rewind again because if we don't do that the delta will not align
                StarfaitByteBuffer_rewind(&buffer, 4);
                // And that's all that there's to it!
            }
        }
    }

    FunctionArrayList_forEach(vm->wad->func.functions, function, i) {
        // This points to the INSTRUCTION address, NOT the operand address, which is why we do +4
        StarfaitByteBuffer_jumpTo(&buffer, (function->firstAddress - vm->wad->code.postAddressPosition) + 4);

        int32_t nextDelta = 0;

        repeat(function->occurrenceCount, j) {
            printf("Processing function %s %d\n", STRGChunk_getString(&vm->wad->strg, function->name), j);

            StarfaitByteBuffer_skip(&buffer, nextDelta);

            FunctionReferenceOperand operand = {.value = StarfaitByteBuffer_readUint32LE(&buffer)};
            nextDelta = FunctionReferenceOperand_delta(operand);

            // We need to rewind because we want to rewrite the operand
            StarfaitByteBuffer_rewind(&buffer, 4);

            // Write the new function index!
            StarfaitByteBuffer_writeInt32LE(&buffer, i); // For functions we don't need to "save" the nibble (w00t!)

            // We rewind again because if we don't do that the delta will not align
            StarfaitByteBuffer_rewind(&buffer, 4);
            // And that's all that there's to it!
        }
    }

    StringArrayList* regularVariableNames = StringArrayList_create(allocatedRegularVariables->size);
    VariableArrayList_forEach(allocatedRegularVariables, variable, i) {
        StarfaitString* string = StarfaitString_create(STRGChunk_getString(&vm->wad->strg, variable->name));
        StringArrayList_add(regularVariableNames, *string);
    }
    vm->regularVariableNames = regularVariableNames;
}

StarfaitVM* StarfaitVM_create(GameWAD* wad) {
    StarfaitVM* vm = calloc(1, sizeof(StarfaitVM));

    vm->callFrameStack = CallFrameArrayList_create(1);
    vm->global = GlobalObject_create();
    vm->builtins = VMBuiltins_create(vm);
    vm->wad = wad;

    remapReferences(vm);

    return vm;
}

static void executeBytecodeInstructions(StarfaitVM* vm, StarfaitByteBuffer* buffer) {
    while (StarfaitByteBuffer_hasRemaining(buffer)) {
        int32_t start = buffer->position;

        OpWord word = {.value = StarfaitByteBuffer_readUint32LE(buffer)};
        Op opcode = OpWord_opcode(word);
        int32_t type1 = OpWord_type1(word);
        int32_t type2 = OpWord_type2(word);
        int32_t extra = OpWord_extra(word);

        // VM: [gml_Object_obj_test_Step_0] (8) [0x4565fff9] POP (type1: 00000005, type2: 00000006, extra: fffffff9) [stack=1 ["Howdy! Loritta is so cute!"]]
        printf("VM: (%d) [%x] %s (type1: %08x, type2: %08x, extra: %08x) [callFrameStack=%d stack=%d", start, word.value, Op_getOpcodeName(opcode), type1, type2, extra, vm->callFrameStack->size, vm->stack.top);
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
                handlePush(vm, buffer, type1, extra);
                break;
            }
            case OP_PUSH_LOCAL: {
                handlePushLocal(vm, buffer, type1, extra);
                break;
            }
            case OP_PUSH_BUILTIN: {
                handlePushBuiltin(vm, buffer, type1);
                break;
            }
            case OP_PUSH_IMMEDIATE: {
                handlePushImmediate(vm, extra);
                break;
            }
            case OP_ADD: {
                handleAdd(vm, buffer, type1, type2);
                break;
            }
            case OP_SUB: {
                handleSub(vm, buffer, type1, type2);
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
            case OP_CMP: {
                handleCmp(vm, buffer, OpWord_comparisonFunction(word), type1, type2);
                break;
            }
            case OP_B: {
                handleB(vm, buffer, OpWord_branchOffset(word));
                break;
            }
            case OP_BT: {
                handleBT(vm, buffer, OpWord_branchOffset(word));
                break;
            }
            case OP_BF: {
                handleBF(vm, buffer, OpWord_branchOffset(word));
                break;
            }
            case OP_DUP: {
                handleDup(vm, buffer, type1, extra);
                break;
            }
            default:
                bye("I don't know how to handle opcode 0x%02X (%s)!", opcode, Op_getOpcodeName(opcode));
        }
    }
}

RValue StarfaitVM_executeCode(StarfaitVM* vm, CodeEntry* code, RValueArrayList* arguments) {
    StarfaitByteBuffer codeBuffer = StarfaitByteBuffer_create(
        // We do + on the offset because the offset is negative
        vm->wad->code.bytecode + (code->offset + (code->bytecodeRelativeOffsetFieldPosition + code->bytecodeRelativeOffset) - vm->wad->code.postAddressPosition),
        code->length - code->offset
    );

    CallFrame* callFrame = CallFrame_create();

    RValueArrayList_forEach(arguments, argument, i) {
        RValueArrayList_set(callFrame->arguments, i, RValue_createCopy(*argument));
    }

    CallFrameArrayList_add(vm->callFrameStack, *callFrame);

    executeBytecodeInstructions(vm, &codeBuffer);

    // Pop the current callFrame
    CallFrameArrayList_removeLast(vm->callFrameStack);

    // TODO: Return result!
    return RValue_createUndefined();
}
