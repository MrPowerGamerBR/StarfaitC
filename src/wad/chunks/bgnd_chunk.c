#include "bgnd_chunk.h"

#include "../background.h"
#include "arraylist_int32.h"
#include "../../starfaitbytebuffer.h"

BGNDChunk BGNDChunk_parse(StarfaitByteBuffer* buffer) {
    Int32ArrayList* addresses = StarfaitByteBuffer_readAddressesAsArrayList(buffer);
    BackgroundArrayList* backgrounds = BackgroundArrayList_create(addresses->size);

    Int32ArrayList_forEachValue(addresses, address, i) {
        StarfaitByteBuffer_jumpTo(buffer, address);

        StringPointer name = StarfaitByteBuffer_readStringPointer(buffer);
        bool transparent = StarfaitByteBuffer_readInt32Boolean(buffer);
        bool smooth = StarfaitByteBuffer_readInt32Boolean(buffer);
        bool preload = StarfaitByteBuffer_readInt32Boolean(buffer);
        TexturePageEntryPointer texturePageEntryPointer = StarfaitByteBuffer_readTexturePageEntryPointer(buffer);

        BackgroundArrayList_add(
            backgrounds,
            (Background){
                .name = name,
                .transparent = transparent,
                .smooth = smooth,
                .preload = preload,
                .texturePageEntryPointer = texturePageEntryPointer
            }
        );
    }

    Int32ArrayList_free(addresses);

    return (BGNDChunk){
        .backgrounds = backgrounds
    };
}
