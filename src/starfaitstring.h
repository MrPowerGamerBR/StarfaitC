#pragma once
#include <stddef.h>

typedef struct {
    char* data;
    size_t length;
} StarfaitString;

StarfaitString* StarfaitString_create(char* original);
StarfaitString* StarfaitString_concat(StarfaitString* left, StarfaitString* right);
char* StarfaitString_toCharArrayView(StarfaitString* string);
void StarfaitString_free(StarfaitString* string);