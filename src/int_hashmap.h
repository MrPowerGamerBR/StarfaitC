#pragma once
#include <stddef.h>
#include <stdint.h>

#include "vm/rvalue.h"

// can i haz bukkit?

typedef struct Int2RValueHashMapEntry {
    uint32_t key;
    RValue value;
    struct Int2RValueHashMapEntry* next;
} Int2RValueHashMapEntry;

typedef struct {
    Int2RValueHashMapEntry** buckets;
    size_t bucketsCount;
} Int2RValueHashMap;

static inline size_t Int2RValueHashMap_hashKey(int key) {
    uint32_t x = (uint32_t) key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

static inline Int2RValueHashMap* Int2RValueHashMap_create(size_t bucketCount) {
    Int2RValueHashMap* map = calloc(1, sizeof(Int2RValueHashMap));
    map->bucketsCount = bucketCount;
    map->buckets = calloc(bucketCount, sizeof(Int2RValueHashMapEntry*));
    return map;
}

static inline RValue* Int2RValueHashMap_get(Int2RValueHashMap* map, uint32_t key) {
    size_t realKey = Int2RValueHashMap_hashKey(key) % map->bucketsCount;

    Int2RValueHashMapEntry* entry = map->buckets[realKey];

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

static inline void Int2RValueHashMap_put(Int2RValueHashMap* map, uint32_t key, RValue value) {
    size_t realKey = Int2RValueHashMap_hashKey(key) % map->bucketsCount;

    Int2RValueHashMapEntry* entry = map->buckets[realKey];
    Int2RValueHashMapEntry* top = entry;

    // Do we already have an entry?
    if (entry != nullptr) {
        while (true) {
            if (entry->key == key) {
                entry->value = value;
            }

            entry = entry->next;
        }
    }

    // Create new entry
    Int2RValueHashMapEntry* newEntry = calloc(1, sizeof(Int2RValueHashMapEntry));
    *newEntry = (Int2RValueHashMapEntry) {
        .key = key,
        .value = value,
        .next = top
    };

    map->buckets[realKey] = newEntry;
}
