#include "arraylist.h"

#include <stdlib.h>

ArrayList* ArrayList_create(size_t initialSize) {
    ArrayList* arrayList = calloc(1, sizeof(ArrayList));
    arrayList->elements = calloc(initialSize, sizeof(void*));
    arrayList->count = 0;
    arrayList->size = initialSize;
    return arrayList;
}

void ArrayList_add(ArrayList* arrayList, void* element) {
    arrayList->elements[arrayList->count++] = element;
}