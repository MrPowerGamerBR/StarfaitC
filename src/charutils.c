#include "charutils.h"

#include <string.h>

bool CharUtils_charEquals(char* s1, char* s2) {
    return strcmp(s1, s2) == 0;
}

char* CharUtils_fancifyBoolean(bool value) {
    return value ? "true" : "false";
}