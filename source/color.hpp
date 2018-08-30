#pragma once

#include <cstdint>

typedef union {
    uint32_t abgr;
    struct
    {
        uint8_t r, g, b, a;
    };
} color_t;
