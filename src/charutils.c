#include "charutils.h"

#include <string.h>

bool CharUtils_charEquals(const char* s1, const char* s2) {
    return strcmp(s1, s2) == 0;
}

char* CharUtils_fancifyBoolean(bool value) {
    return value ? "true" : "false";
}