#include <stddef.h>
#include <stdint.h>

#include "vm/rvalue.h"

// can i haz bukkit?

// Used just for testing purposes just so that CLion doesn't go haywire
#ifndef HASH_MAP_NAME
#define HASH_MAP_NAME Int2RValueHashMap
#define HASH_MAP_TYPE RValue
#endif

#ifndef MAP_CONCAT
#define MAP_CONCAT_(a, b) a##b
#define MAP_CONCAT(a, b) MAP_CONCAT_(a, b)
#endif

#define FN(name) MAP_CONCAT(HASH_MAP_NAME, _##name)
#define HASH_MAP_ENTRY_NAME MAP_CONCAT(HASH_MAP_NAME, Entry)

typedef struct HASH_MAP_ENTRY_NAME {
    int32_t key;
    HASH_MAP_TYPE value;
    struct HASH_MAP_ENTRY_NAME* next;
} HASH_MAP_ENTRY_NAME;

typedef struct HASH_MAP_NAME {
    HASH_MAP_ENTRY_NAME** buckets;
    size_t bucketsCount;
} HASH_MAP_NAME;

static inline size_t FN(hashKey)(int key) {
    uint32_t x = (uint32_t) key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

static inline HASH_MAP_NAME* FN(create)(size_t bucketCount) {
    HASH_MAP_NAME* map = calloc(1, sizeof(HASH_MAP_NAME));
    map->bucketsCount = bucketCount;
    map->buckets = calloc(bucketCount, sizeof(HASH_MAP_ENTRY_NAME*));
    return map;
}

static inline RValue* FN(get)(HASH_MAP_NAME* map, int32_t key) {
    size_t realKey = Int2RValueHashMap_hashKey(key) % map->bucketsCount;

    HASH_MAP_ENTRY_NAME* entry = map->buckets[realKey];

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

static inline void FN(put)(HASH_MAP_NAME* map, int32_t key, RValue value) {
    size_t realKey = Int2RValueHashMap_hashKey(key) % map->bucketsCount;

    HASH_MAP_ENTRY_NAME* entry = map->buckets[realKey];
    HASH_MAP_ENTRY_NAME* top = entry;

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
    HASH_MAP_ENTRY_NAME* newEntry = calloc(1, sizeof(HASH_MAP_ENTRY_NAME));
    *newEntry = (HASH_MAP_ENTRY_NAME) {
        .key = key,
        .value = value,
        .next = top
    };

    map->buckets[realKey] = newEntry;
}

#undef FN
#undef HASH_MAP_ENTRY_NAME
#undef HASH_MAP_NAME
#undef HASH_MAP_TYPE
#undef MAP_CONCAT
#undef MAP_CONCAT_