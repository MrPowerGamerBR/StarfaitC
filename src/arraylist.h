#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mathutils.h"

// We set a ifdef for allll of these things just so that a stray arraylist.h doesn't cause duplicated lists
#ifdef ARRAY_LIST_NAME
#ifndef LIST_CONCAT
#define LIST_CONCAT_(a, b) a##b
#define LIST_CONCAT(a, b)  LIST_CONCAT_(a, b)
#endif

#define FN(name) LIST_CONCAT(ARRAY_LIST_NAME, _##name)

typedef struct {
    ARRAY_LIST_TYPE* elements;
    size_t size;
    size_t capacity;
} ARRAY_LIST_NAME;

static inline ARRAY_LIST_NAME* FN(create)(size_t initialSize) {
    ARRAY_LIST_NAME* arrayList = calloc(1, sizeof(ARRAY_LIST_NAME));

    arrayList->elements = calloc(initialSize, sizeof(ARRAY_LIST_TYPE));
    arrayList->size = 0;
    arrayList->capacity = initialSize;
    return arrayList;
}

static inline void FN(add)(ARRAY_LIST_NAME* list, ARRAY_LIST_TYPE element) {
    if (list->capacity == list->size) {
        // Double the size!
        uint32_t newCapacity = MathUtils_max(2, list->capacity * 2);
        ARRAY_LIST_TYPE* newElements = calloc(newCapacity, sizeof(ARRAY_LIST_TYPE));
        memcpy(newElements, list->elements, list->capacity * sizeof(ARRAY_LIST_TYPE));
        free(list->elements);
        list->elements = newElements;
        list->capacity = newCapacity;
    }
    list->elements[list->size] = element;
    list->size++;
}

static inline void FN(free)(ARRAY_LIST_NAME* list) {
    free(list->elements);
    free(list);
}

#undef FN
#undef ARRAY_LIST_NAME
#undef ARRAY_LIST_TYPE
#undef LIST_CONCAT
#undef LIST_CONCAT_

#endif