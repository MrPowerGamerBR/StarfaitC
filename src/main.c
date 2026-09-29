#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "charutils.h"
#include "starfaitbytebuffer.h"
#include "starfaitstring.h"
#include "utils.h"
#include "vm/vm.h"
#include "wad/code_chunk.h"
#include "wad/func_chunk.h"
#include "wad/gen8_chunk.h"
#include "wad/strg_chunk.h"
#include "wad/vari_chunk.h"
#include "wad/wad.h"

int main() {
    // FILE* file = fopen("/home/mrpowergamerbr/Projects/ButterGMGames/Undertale_108/data.win", "rb");
    FILE* file = fopen("/home/mrpowergamerbr/Documentos/VMShare/TestStarfaitC23Minimal-Default-1.0.0.12/data.win", "rb");

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
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

    StarfaitByteBuffer byteBuffer = StarfaitByteBuffer_create(buf, size);
    GameWAD gameWAD = GameWAD_parse(&byteBuffer);

    StarfaitVM* vm = StarfaitVM_create(&gameWAD);

    StarfaitVM_executeCode(vm, CodeEntryArrayList_get(vm->wad->code.codeEntries, 1), RValueArrayList_create(0));
    return 0;
}
