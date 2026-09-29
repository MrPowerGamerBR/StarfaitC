#pragma once

#include <stddef.h>
#include <stdint.h>
#include "vm/rvalue.h"

// can i haz bukkit?

typedef struct __HASH_MAP_NAME__Entry {
    int32_t key;
    __HASH_MAP_ENTRY_TYPE__ value;
    struct __HASH_MAP_NAME__Entry* next;
} __HASH_MAP_NAME__Entry;

typedef struct __HASH_MAP_NAME__ {
    __HASH_MAP_NAME__Entry** buckets;
    size_t bucketsCount;
} __HASH_MAP_NAME__;

static inline size_t __HASH_MAP_NAME___hashKey(int key) {
    uint32_t x = (uint32_t) key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

static inline __HASH_MAP_NAME__* __HASH_MAP_NAME___create(size_t bucketCount) {
    __HASH_MAP_NAME__* map = calloc(1, sizeof(__HASH_MAP_NAME__));
    map->bucketsCount = bucketCount;
    map->buckets = calloc(bucketCount, sizeof(__HASH_MAP_ENTRY_TYPE__*));
    return map;
}

static inline RValue* __HASH_MAP_NAME___get(__HASH_MAP_NAME__* map, int32_t key) {
    size_t realKey = __HASH_MAP_NAME___hashKey(key) % map->bucketsCount;

    __HASH_MAP_NAME__Entry* entry = map->buckets[realKey];

    if (entry == nullptr)
        return nullptr;

    // Iterate over until we find what we want
    while (true) {
        if (entry->key == key)
            return &entry->value;

        entry = entry->next;
    }

    return nullptr;
}

static inline void __HASH_MAP_NAME___put(__HASH_MAP_NAME__* map, int32_t key, RValue value) {
    size_t realKey = __HASH_MAP_NAME___hashKey(key) % map->bucketsCount;

    __HASH_MAP_NAME__Entry* entry = map->buckets[realKey];
    __HASH_MAP_NAME__Entry* top = entry;

    // Do we already have an entry?
    while (true) {
        if (entry == nullptr)
            break;

        if (entry->key == key) {
            entry->value = value;
        }

        entry = entry->next;
    }

    // Create new entry
    __HASH_MAP_NAME__Entry* newEntry = calloc(1, sizeof(__HASH_MAP_NAME__Entry));
    *newEntry = (__HASH_MAP_NAME__Entry) {
        .key = key,
        .value = value,
        .next = top
    };

    map->buckets[realKey] = newEntry;
}