#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct {
    char* data;
    size_t length;
} String;

void String_append(String* string, char* data, size_t size);