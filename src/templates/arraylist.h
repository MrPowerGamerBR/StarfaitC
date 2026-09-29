#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mathutils.h"
#include "utils.h"
__INCLUDES__

typedef struct {
    __ARRAY_LIST_TYPE__* elements;
    size_t size;
    size_t capacity;
} __ARRAY_LIST_NAME__;

static inline __ARRAY_LIST_NAME__* __ARRAY_LIST_NAME___create(size_t initialSize) {
    __ARRAY_LIST_NAME__* arrayList = calloc(1, sizeof(__ARRAY_LIST_NAME__));

    arrayList->elements = calloc(initialSize, sizeof(__ARRAY_LIST_TYPE__));
    arrayList->size = 0;
    arrayList->capacity = initialSize;
    return arrayList;
}

static inline void __ARRAY_LIST_NAME___add(__ARRAY_LIST_NAME__* list, __ARRAY_LIST_TYPE__ element) {
    if (list->capacity == list->size) {
        // Double the size!
        uint32_t newCapacity = MathUtils_max(2, list->capacity * 2);
        __ARRAY_LIST_TYPE__* newElements = calloc(newCapacity, sizeof(__ARRAY_LIST_TYPE__));
        memcpy(newElements, list->elements, list->capacity * sizeof(__ARRAY_LIST_TYPE__));
        free(list->elements);
        list->elements = newElements;
        list->capacity = newCapacity;
    }
    list->elements[list->size] = element;
    list->size++;
}

static inline __ARRAY_LIST_TYPE__* __ARRAY_LIST_NAME___get(__ARRAY_LIST_NAME__* list, uint32_t index) {
    require(list->size > index);
    return &list->elements[index];
}

static inline void __ARRAY_LIST_NAME___free(__ARRAY_LIST_NAME__* list) {
    free(list->elements);
    free(list);
}

// This is a super duper hack!!
// But essentially we first iterate by the index, then we do a fake "for" that only sets the ptr to what we want
#define __ARRAY_LIST_NAME___forEach(list, ptr, i) \
    for (size_t i = 0; list->size > i; i++) \
        for (__ARRAY_LIST_TYPE__* ptr = &list->elements[i]; ptr != nullptr; ptr = nullptr)
