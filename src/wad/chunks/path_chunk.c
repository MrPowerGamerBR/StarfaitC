#include "path_chunk.h"

#include "../path.h"
#include "../pathpoint.h"
#include "arraylist_uint32.h"
#include "../../starfaitbytebuffer.h"
#include "../../utils.h"

PATHChunk PATHChunk_parse(StarfaitByteBuffer* buffer) {
    Uint32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);
    PathArrayList* paths = PathArrayList_create(addresses->size);

    Uint32ArrayList_forEach(addresses, address, i) {
        StarfaitByteBuffer_jumpTo(buffer, *address);

        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        int32_t kind = StarfaitByteBuffer_readInt32LE(buffer);
        bool closed = StarfaitByteBuffer_readInt32Boolean(buffer);
        int32_t precision = StarfaitByteBuffer_readInt32LE(buffer);
        uint32_t pointCount = StarfaitByteBuffer_readUint32LE(buffer);

        PathPointArrayList* points = PathPointArrayList_create(pointCount);
        repeat(pointCount, j) {
            float x = StarfaitByteBuffer_readFloatLE(buffer);
            float y = StarfaitByteBuffer_readFloatLE(buffer);
            float speed = StarfaitByteBuffer_readFloatLE(buffer);

            PathPointArrayList_add(
                points,
                (PathPoint){
                    .x = x,
                    .y = y,
                    .speed = speed
                }
            );
        }

        PathArrayList_add(
            paths,
            (Path){
                .name = name,
                .kind = kind,
                .closed = closed,
                .precision = precision,
                .points = points
            }
        );
    }

    Uint32ArrayList_free(addresses);

    return (PATHChunk){
        .paths = paths
    };
}
