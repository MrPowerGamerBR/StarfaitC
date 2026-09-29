#include "starfaitstrings.h"

#include <stdlib.h>
#include <string.h>

void String_append(String* string, char* data, size_t size) {
    char* newData = malloc(string->length + size);

    if (string->data != nullptr) {
        memcpy(newData, data, string->length);
        free(string->data);
    }

    memcpy(newData + string->length, data, size);
    string->data = newData;
    string->length = string->length + size;
}
