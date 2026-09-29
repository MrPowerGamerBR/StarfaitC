#pragma once
#include <stddef.h>
#include <stdint.h>

struct {
    void** elements;
    size_t count;
    size_t size;
} typedef ArrayList;

ArrayList* ArrayList_create(size_t initialSize);
void ArrayList_add(ArrayList* arrayList, void* element);