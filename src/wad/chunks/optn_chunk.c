#include "strg_chunk.h"

#include <stdio.h>
#include <stdlib.h>

#include "optn_chunk.h"

#include "../../starfaitbytebuffer.h"
#include "../../utils.h"

OPTNChunk OPTNChunk_parse(StarfaitByteBuffer* buffer) {
    StarfaitByteBuffer_readInt32LE(buffer); // magic
    StarfaitByteBuffer_readInt32LE(buffer); // format version
    auto flags = StarfaitByteBuffer_readUint32LE(buffer);
    StarfaitByteBuffer_readInt32LE(buffer); // unused
    auto scale = StarfaitByteBuffer_readInt32LE(buffer);
    auto windowColor = StarfaitByteBuffer_readInt32LE(buffer);
    auto colorDepth = StarfaitByteBuffer_readInt32LE(buffer);
    auto resolution = StarfaitByteBuffer_readInt32LE(buffer);
    auto frequency = StarfaitByteBuffer_readInt32LE(buffer);
    auto vertexSync = StarfaitByteBuffer_readInt32LE(buffer);
    auto priority = StarfaitByteBuffer_readInt32LE(buffer);
    StarfaitByteBuffer_readInt32LE(buffer); // unused
    StarfaitByteBuffer_readInt32LE(buffer); // unused
    auto loadImage = StarfaitByteBuffer_readInt32LE(buffer); // this is a pointer
    auto loadAlpha = StarfaitByteBuffer_readInt32LE(buffer);
    auto constantCount = StarfaitByteBuffer_readInt32LE(buffer);
    auto constants = OptnConstantArrayList_create(constantCount);

    repeat(constantCount, i) {
        auto name = StarfaitByteBuffer_readStringPointer(buffer);
        auto value = StarfaitByteBuffer_readStringPointer(buffer);
        OptnConstantArrayList_add(
            constants,
            (OptnConstant){
                .name = name,
                .value = value
            }
        );
    }

    return (OPTNChunk){
        .flags = (OptnFlags){.value = flags},
        .scale = scale,
        .colorDepth = colorDepth,
        .windowColor = windowColor,
        .resolution = resolution,
        .frequency = frequency,
        .vertexSync = vertexSync,
        .priority = priority,
        .loadImage = loadImage,
        .loadAlpha = loadAlpha
    };
}