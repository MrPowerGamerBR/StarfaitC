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
    int32_t size;
    int32_t capacity;
} __ARRAY_LIST_NAME__;

static inline __ARRAY_LIST_NAME__* __ARRAY_LIST_NAME___create(int32_t initialSize) {
    require(initialSize >= 0, "You can't create a negative (%d) capacity list!", initialSize);
    __ARRAY_LIST_NAME__* arrayList = calloc(1, sizeof(__ARRAY_LIST_NAME__));

    arrayList->elements = calloc((size_t) initialSize, sizeof(__ARRAY_LIST_TYPE__));
    arrayList->size = 0;
    arrayList->capacity = initialSize;
    return arrayList;
}

static inline void __ARRAY_LIST_NAME___add(__ARRAY_LIST_NAME__* list, __ARRAY_LIST_TYPE__ element) {
    if (list->capacity == list->size) {
        // Double the size!
        int32_t newCapacity = MathUtils_maxInt32(2, list->capacity * 2);
        __ARRAY_LIST_TYPE__* newElements = calloc((size_t) newCapacity, sizeof(__ARRAY_LIST_TYPE__));
        memcpy(newElements, list->elements, (size_t) list->capacity * sizeof(__ARRAY_LIST_TYPE__));
        free(list->elements);
        list->elements = newElements;
        list->capacity = newCapacity;
    }
    list->elements[list->size] = element;
    list->size++;
}

static inline void __ARRAY_LIST_NAME___set(__ARRAY_LIST_NAME__* list, int32_t index, __ARRAY_LIST_TYPE__ element) {
    require(index >= 0 && list->size > index, "Index %d out of bounds for size %d", index, list->size);
    require((int32_t) list->size > index);
    list->elements[index] = element;
}

static inline __ARRAY_LIST_TYPE__* __ARRAY_LIST_NAME___last(__ARRAY_LIST_NAME__* list) {
    require(list->size != 0);
    return &list->elements[list->size - 1];
}

static inline __ARRAY_LIST_TYPE__* __ARRAY_LIST_NAME___removeLast(__ARRAY_LIST_NAME__* list) {
    require(list->size != 0);
    return &list->elements[--list->size];
}

static inline __ARRAY_LIST_TYPE__* __ARRAY_LIST_NAME___get(__ARRAY_LIST_NAME__* list, int32_t index) {
    require(index >= 0 && list->size > index, "Index %d out of bounds for size %d", index, list->size);
    require((int32_t) list->size > index);
    return &list->elements[index];
}

static inline __ARRAY_LIST_NAME__* __ARRAY_LIST_NAME___createFromCArray(__ARRAY_LIST_TYPE__* cArray, int32_t size) {
    __ARRAY_LIST_NAME__* arrayList = __ARRAY_LIST_NAME___create(size);

    for (int32_t i = 0; size > i; i++) {
        __ARRAY_LIST_NAME___add(arrayList, cArray[i]);
    }

    return arrayList;
}

static inline void __ARRAY_LIST_NAME___free(__ARRAY_LIST_NAME__* list) {
    free(list->elements);
    free(list);
}

__ADDITIONAL_FUNCTIONS__

// This is a super duper hack!!
// But essentially we first iterate by the index, then we do a fake "for" that only sets the ptr to what we want
#define __ARRAY_LIST_NAME___forEach(list, ptr, i) \
    for (int32_t i = 0; list->size > i; i++) \
        for (__ARRAY_LIST_TYPE__* ptr = &list->elements[i]; ptr != nullptr; ptr = nullptr)
