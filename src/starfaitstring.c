#include "starfaitstring.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct StarfaitString {
    char* data;
    size_t length;
};

StarfaitString* createWithLength(size_t length) {
    StarfaitString* string = calloc(1, sizeof(StarfaitString));
    string->data = calloc(length + 1, sizeof(char));
    string->length = length;
    return string;
}

StarfaitString* StarfaitString_create(char* original) {
    size_t originalLength = strlen(original);
    StarfaitString* string = createWithLength(originalLength);
    memcpy(string->data, original, originalLength);
    return string;
}

StarfaitString* StarfaitString_concat(StarfaitString* left, StarfaitString* right) {
    size_t newLength = left->length + right->length;

    StarfaitString* string = createWithLength(newLength);
    memcpy(string->data, left->data, left->length);
    memcpy(string->data + left->length, right->data, right->length);
    return string;
}

char* StarfaitString_toCharArrayView(StarfaitString* string) {
    return string->data;
}

void StarfaitString_free(StarfaitString* string) {
    free(string->data);
    free(string);
}