#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "charutils.h"
#include "starfaitbytebuffer.h"
#include "starfaitstring.h"
#include "utils.h"
#include "vm/vm.h"
#include "wad/chunks/code_chunk.h"
#include "wad/chunks/func_chunk.h"
#include "wad/chunks/gen8_chunk.h"
#include "wad/chunks/strg_chunk.h"
#include "wad/chunks/vari_chunk.h"
#include "wad/wad.h"

int main() {
    // FILE* file = fopen("/home/mrpowergamerbr/Projects/ButterGMGames/Undertale_108/data.win", "rb");
    FILE* file = fopen("/home/mrpowergamerbr/Documentos/VMShare/TestStarfaitC23Minimal-Default-1.0.0.14/data.win", "rb");

    // Get file size
    fseek(file, 0, SEEK_END);
    long status = ftell(file);
    size_t size = (size_t) status;
    rewind(file);

    // Read the whole file into memory
    uint8_t* buf = malloc(size);
    if (!buf || fread(buf, 1, size, file) != (size_t)size) {
        perror("read");
        free(buf);
        fclose(file);
        return 1;
    }
    fclose(file);

    StarfaitByteBuffer byteBuffer = StarfaitByteBuffer_create(buf, (int32_t) size);
    GameWAD gameWAD = GameWAD_parse(&byteBuffer);

    StarfaitVM* vm = StarfaitVM_create(&gameWAD);

    StarfaitVM_executeCode(vm, CodeEntryArrayList_get(vm->wad->code.codeEntries, 1), RValueArrayList_create(0));
    return 0;
}
