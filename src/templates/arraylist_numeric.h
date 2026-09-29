static inline __ARRAY_LIST_TYPE__ __ARRAY_LIST_NAME___min(__ARRAY_LIST_NAME__* list) {
    require(list->size != 0);

    __ARRAY_LIST_TYPE__ minValue = list->elements[0];

    repeat(list->size, i) {
        if (minValue > list->elements[i]) {
            minValue = list->elements[i];
        }
    }

    return minValue;
}