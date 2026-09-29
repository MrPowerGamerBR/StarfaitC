#pragma once
#include <stddef.h>
#include <stdint.h>

#include "wad/stringpointer.h"

typedef struct {
    uint8_t* data;
    size_t size;
    size_t position;
} StarfaitByteBuffer;

StarfaitByteBuffer StarfaitByteBuffer_create(uint8_t* data, size_t size);

uint8_t StarfaitByteBuffer_readUint8(StarfaitByteBuffer* buffer);
bool StarfaitByteBuffer_readUint8Boolean(StarfaitByteBuffer* buffer);
uint16_t StarfaitByteBuffer_readUint16LE(StarfaitByteBuffer* buffer);
uint32_t StarfaitByteBuffer_readUint32LE(StarfaitByteBuffer* buffer);
void StarfaitByteBuffer_writeUint32LE(StarfaitByteBuffer* buffer, uint32_t value);
uint64_t StarfaitByteBuffer_readUint64LE(StarfaitByteBuffer* buffer);
StringPointer StarfaitByteBuffer_readStringPointer(StarfaitByteBuffer* buffer);
void StarfaitByteBuffer_readAddresses(StarfaitByteBuffer* buffer, size_t* outCount, uint32_t** outAddresses);
uint8_t* StarfaitByteBuffer_readBytes(StarfaitByteBuffer* buffer, size_t count);
char* StarfaitByteBuffer_readChars(StarfaitByteBuffer* buffer, size_t count);
void StarfaitByteBuffer_skip(StarfaitByteBuffer* buffer, size_t count);
void StarfaitByteBuffer_rewind(StarfaitByteBuffer* buffer, size_t count);
void StarfaitByteBuffer_jumpTo(StarfaitByteBuffer* buffer, size_t newPosition);
bool StarfaitByteBuffer_hasRemaining(StarfaitByteBuffer* buffer);