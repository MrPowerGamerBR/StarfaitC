#pragma once
#include <stdint.h>

#include "code.h"
#include "func.h"
#include "gen8.h"
#include "scpt.h"
#include "strg.h"
#include "vari.h"

typedef struct {
    GEN8 gen8;
    VARI vari;
    STRG strg;
    FUNC func;
    CODE code;
    SCPT scpt;
} GameWAD;

GameWAD GameWAD_parse(StarfaitByteBuffer* buffer);