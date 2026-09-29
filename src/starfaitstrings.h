#pragma once
#include <stddef.h>
#include <stdint.h>

struct {
    char* data;
    size_t length;
} typedef String;

void String_append(String* string, char* data, size_t size);