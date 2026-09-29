#include "starfaitbytebuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "wad/wad.h"

StarfaitByteBuffer StarfaitByteBuffer_create(uint8_t* data, int32_t size) {
    return (StarfaitByteBuffer){
        .data = data,
        .size = size,
        .position = 0
    };
}

uint8_t StarfaitByteBuffer_readUint8(StarfaitByteBuffer* buffer) {
    return buffer->data[buffer->position++];
}

bool StarfaitByteBuffer_readUint8Boolean(StarfaitByteBuffer* buffer) {
    return StarfaitByteBuffer_readUint8(buffer) == 1;
}

uint16_t StarfaitByteBuffer_readUint16LE(StarfaitByteBuffer* buffer) {
    const uint32_t v = (uint32_t) buffer->data[buffer->position] | ((uint32_t) buffer->data[buffer->position + 1] << 8);
    buffer->position += 2;
    return (uint16_t) v;
}

uint32_t StarfaitByteBuffer_readUint32LE(StarfaitByteBuffer* buffer) {
    const uint32_t v = (uint32_t) buffer->data[buffer->position] | ((uint32_t) buffer->data[buffer->position + 1] << 8) | ((uint32_t) buffer->data[buffer->position + 2] << 16) | ((uint32_t) buffer->data[buffer->position + 3] << 24);
    buffer->position += 4;
    return v;
}

int32_t StarfaitByteBuffer_readInt32LE(StarfaitByteBuffer* buffer) {
    return (int32_t) StarfaitByteBuffer_readUint32LE(buffer);
}

bool StarfaitByteBuffer_readInt32Boolean(StarfaitByteBuffer* buffer) {
    return StarfaitByteBuffer_readInt32LE(buffer) != 0;
}

float StarfaitByteBuffer_readFloatLE(StarfaitByteBuffer* buffer) {
    const uint32_t bits = StarfaitByteBuffer_readUint32LE(buffer);
    float value;
    memcpy(&value, &bits, sizeof(float));
    return value;
}

void StarfaitByteBuffer_writeUint32LE(StarfaitByteBuffer* buffer, uint32_t value) {
    // YOU NEED TO EXPLICITLY ADD THOSE DAMN () BECAUSE IF YOU DON'T, THE VALUE WILL BE CASTED BEFORE THE BITWISE OPERATION!!!
    buffer->data[buffer->position++] = (uint8_t) (value);
    buffer->data[buffer->position++] = (uint8_t) (value >> 8);
    buffer->data[buffer->position++] = (uint8_t) (value >> 16);
    buffer->data[buffer->position++] = (uint8_t) (value >> 24);
}

void StarfaitByteBuffer_writeInt32LE(StarfaitByteBuffer* buffer, int32_t value) {
    StarfaitByteBuffer_writeUint32LE(buffer, (uint32_t) value);
}

uint64_t StarfaitByteBuffer_readUint64LE(StarfaitByteBuffer* buffer) {
    uint64_t v = 0;
    for (int i = 7; i >= 0; i--) {
        v = (v << 8) | buffer->data[buffer->position + i];
    }
    buffer->position += 8;
    return v;
}

StringPointer StarfaitByteBuffer_readStringPointer(StarfaitByteBuffer* buffer) {
    return (StringPointer){.value = StarfaitByteBuffer_readInt32LE(buffer)};
}

TexturePageEntryPointer StarfaitByteBuffer_readTexturePageEntryPointer(StarfaitByteBuffer* buffer) {
    return (TexturePageEntryPointer){.value = StarfaitByteBuffer_readInt32LE(buffer)};
}

void StarfaitByteBuffer_readAddresses(StarfaitByteBuffer* buffer, int32_t* outCount, int32_t** outAddresses) {
    int32_t addressesCount = StarfaitByteBuffer_readInt32LE(buffer);
    *outCount = addressesCount;
    int32_t* addresses = calloc((size_t) addressesCount, sizeof(int32_t));

    repeat(addressesCount, i) {
        addresses[i] = StarfaitByteBuffer_readInt32LE(buffer);
    }

    *outAddresses = addresses;
}

Int32ArrayList* StarfaitByteBuffer_readAddressesAsArrayList(StarfaitByteBuffer* buffer) {
    int32_t count;
    int32_t* addresses;

    StarfaitByteBuffer_readAddresses(buffer, &count, &addresses);

    Int32ArrayList* arrayList = Int32ArrayList_create(count);

    repeat(count, i) {
        Int32ArrayList_add(arrayList, addresses[i]);
    }

    return arrayList;
}

uint8_t* StarfaitByteBuffer_readBytes(StarfaitByteBuffer* buffer, int32_t count) {
    uint8_t* data = malloc((size_t) count);
    memcpy(data, buffer->data + buffer->position, (size_t) count);
    buffer->position += count;
    return data;
}

char* StarfaitByteBuffer_readChars(StarfaitByteBuffer* buffer, int32_t count) {
    // All of this just so that the char* ends with a \0 smh
    char* chars = calloc((size_t) (count + 1), sizeof(char));
    uint8_t* output = StarfaitByteBuffer_readBytes(buffer, count);
    memcpy(chars, output, (size_t) count);
    return chars;
}

void StarfaitByteBuffer_skip(StarfaitByteBuffer* buffer, int32_t count) {
    buffer->position += count;
}

void StarfaitByteBuffer_rewind(StarfaitByteBuffer* buffer, int32_t count) {
    buffer->position -= count;
}

void StarfaitByteBuffer_jumpTo(StarfaitByteBuffer* buffer, int32_t newPosition) {
    buffer->position = newPosition;
}

bool StarfaitByteBuffer_hasRemaining(StarfaitByteBuffer* buffer) {
    return buffer->size > buffer->position;
}
