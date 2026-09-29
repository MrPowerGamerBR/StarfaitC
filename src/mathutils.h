#pragma once

static inline uint32_t MathUtils_maxUint32(uint32_t val1, uint32_t val2) {
    return val1 > val2 ? val1 : val2;
}

static inline int32_t MathUtils_maxInt32(int32_t val1, int32_t val2) {
    return val1 > val2 ? val1 : val2;
}