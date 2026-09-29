#pragma once
#include "script.h"
#include "../starfaitbytebuffer.h"
#include "arraylist_script.h"

typedef struct {
    ScriptArrayList* scripts;
} SCPT;

SCPT SCPT_parse(StarfaitByteBuffer* buffer);