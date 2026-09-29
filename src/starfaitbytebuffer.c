#include "starfaitbytebuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "wad/wad.h"

StarfaitByteBuffer StarfaitByteBuffer_create(uint8_t* data, size_t size) {
    return (StarfaitByteBuffer) {
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
    return v;
}

uint32_t StarfaitByteBuffer_readUint32LE(StarfaitByteBuffer* buffer) {
    const uint32_t v = (uint32_t) buffer->data[buffer->position] | ((uint32_t) buffer->data[buffer->position + 1] << 8) | ((uint32_t) buffer->data[buffer->position + 2] << 16) | ((uint32_t) buffer->data[buffer->position + 3] << 24);
    buffer->position += 4;
    return v;
}

int32_t StarfaitByteBuffer_readInt32LE(StarfaitByteBuffer* buffer) {
    return (int32_t) StarfaitByteBuffer_readUint32LE(buffer);
}

void StarfaitByteBuffer_writeUint32LE(StarfaitByteBuffer* buffer, uint32_t value) {
    // YOU NEED TO EXPLICITLY ADD THOSE DAMN () BECAUSE IF YOU DON'T, THE VALUE WILL BE CASTED BEFORE THE BITWISE OPERATION!!!
    buffer->data[buffer->position++] = (uint8_t) (value);
    buffer->data[buffer->position++] = (uint8_t) (value >> 8);
    buffer->data[buffer->position++] = (uint8_t) (value >> 16);
    buffer->data[buffer->position++] = (uint8_t) (value >> 24);
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
    return (StringPointer) { .value = StarfaitByteBuffer_readUint32LE(buffer) };
}

void StarfaitByteBuffer_readAddresses(StarfaitByteBuffer* buffer, size_t* outCount, uint32_t** outAddresses) {
    uint32_t addressesCount = StarfaitByteBuffer_readUint32LE(buffer);
    *outCount = addressesCount;
    uint32_t* addresses = calloc(addressesCount, sizeof(uint32_t));

    repeat(addressesCount, i) {
        addresses[i] = StarfaitByteBuffer_readUint32LE(buffer);
    }

    *outAddresses = addresses;
}

Uint32ArrayList* StarfaitByteBuffer_readAddressesAsArrayList(StarfaitByteBuffer* buffer) {
    size_t count;
    uint32_t* addresses;

    StarfaitByteBuffer_readAddresses(buffer, &count, &addresses);

    Uint32ArrayList* arrayList = Uint32ArrayList_create(count);

    repeat(count, i) {
        Uint32ArrayList_add(arrayList, addresses[i]);
    }

    return arrayList;
}

uint8_t* StarfaitByteBuffer_readBytes(StarfaitByteBuffer* buffer, size_t count) {
    uint8_t* data = malloc(count);
    memcpy(data, buffer->data + buffer->position, count);
    buffer->position += count;
    return data;
}

char* StarfaitByteBuffer_readChars(StarfaitByteBuffer* buffer, size_t count) {
    // All of this just so that the char* ends with a \0 smh
    char* chars = calloc(count + 1, sizeof(char));
    uint8_t* output = StarfaitByteBuffer_readBytes(buffer, count);
    memcpy(chars, output, count);
    return chars;
}

void StarfaitByteBuffer_skip(StarfaitByteBuffer* buffer, size_t count) {
    buffer->position += count;
}

void StarfaitByteBuffer_rewind(StarfaitByteBuffer* buffer, size_t count) {
    buffer->position -= count;
}

void StarfaitByteBuffer_jumpTo(StarfaitByteBuffer* buffer, size_t newPosition) {
    buffer->position = newPosition;
}

bool StarfaitByteBuffer_hasRemaining(StarfaitByteBuffer* buffer) {
    return buffer->size > buffer->position;
}