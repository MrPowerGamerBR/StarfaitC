#pragma once
#include <stddef.h>
#include <stdint.h>

#include "wad/stringpointer.h"
#include "wad/texturepageentrypointer.h"
#include "arraylist_int32.h"

typedef struct {
    uint8_t* data;
    int32_t size;
    int32_t position;
} StarfaitByteBuffer;

StarfaitByteBuffer StarfaitByteBuffer_create(uint8_t* data, int32_t size);

uint8_t StarfaitByteBuffer_readUint8(StarfaitByteBuffer* buffer);
bool StarfaitByteBuffer_readUint8Boolean(StarfaitByteBuffer* buffer);
bool StarfaitByteBuffer_readInt32Boolean(StarfaitByteBuffer* buffer);
float StarfaitByteBuffer_readFloatLE(StarfaitByteBuffer* buffer);
uint16_t StarfaitByteBuffer_readUint16LE(StarfaitByteBuffer* buffer);
int32_t StarfaitByteBuffer_readInt32LE(StarfaitByteBuffer* buffer);
uint32_t StarfaitByteBuffer_readUint32LE(StarfaitByteBuffer* buffer);
void StarfaitByteBuffer_writeUint32LE(StarfaitByteBuffer* buffer, uint32_t value);
void StarfaitByteBuffer_writeInt32LE(StarfaitByteBuffer* buffer, int32_t value);
uint64_t StarfaitByteBuffer_readUint64LE(StarfaitByteBuffer* buffer);
StringPointer StarfaitByteBuffer_readStringPointer(StarfaitByteBuffer* buffer);
TexturePageEntryPointer StarfaitByteBuffer_readTexturePageEntryPointer(StarfaitByteBuffer* buffer);
void StarfaitByteBuffer_readAddresses(StarfaitByteBuffer* buffer, int32_t* outCount, int32_t** outAddresses);
Int32ArrayList* StarfaitByteBuffer_readAddressesAsArrayList(StarfaitByteBuffer* buffer);
uint8_t* StarfaitByteBuffer_readBytes(StarfaitByteBuffer* buffer, int32_t count);
char* StarfaitByteBuffer_readChars(StarfaitByteBuffer* buffer, int32_t count);
void StarfaitByteBuffer_skip(StarfaitByteBuffer* buffer, int32_t count);
void StarfaitByteBuffer_rewind(StarfaitByteBuffer* buffer, int32_t count);
void StarfaitByteBuffer_jumpTo(StarfaitByteBuffer* buffer, int32_t newPosition);
bool StarfaitByteBuffer_hasRemaining(StarfaitByteBuffer* buffer);