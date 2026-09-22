#ifndef MORTON_H
#define MORTON_H

#include <cassert>
#include <cstdint>

inline uint64_t expand(uint64_t input, uint32_t scale) {
    assert(scale > 0);
    if (scale == 1) {
        return input;
    }

    const uint32_t maxLen = 64;
    uint64_t newVal = 0;

    for (uint32_t i = 0; i < maxLen; ++i) {
        uint64_t currBit  = (input >> i) & 1;
        uint64_t newPos = i * scale;

        if (newPos < maxLen) {
            currBit <<= newPos;
            newVal |= currBit;
        }
    }

    return newVal;
}

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t result = 0;
    uint64_t x2 = expand(x, 3);
    uint64_t y2 = expand(y, 3) << 1;
    uint64_t z2 = expand(z, 3) << 2;
    result = x2 | y2 | z2;

    return result;
}

#endif  // MORTON_H