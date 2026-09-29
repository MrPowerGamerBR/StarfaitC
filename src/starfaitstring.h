#pragma once

typedef struct StarfaitString StarfaitString;

StarfaitString* StarfaitString_create(char* original);
StarfaitString* StarfaitString_concat(StarfaitString* left, StarfaitString* right);
char* StarfaitString_toCharArrayView(StarfaitString* string);
void StarfaitString_free(StarfaitString* string);