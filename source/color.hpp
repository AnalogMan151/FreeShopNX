#pragma once

#include <switch/types.h>
#include "ui.hpp"

typedef union {
    u32 abgr;
    struct
    {
        u8 r, g, b, a;
    };
} color_t;

void DrawPixel(frame_t& frame, u32 x, u32 y, color_t clr);
void DrawPixelRaw(frame_t& frame, u32 x, u32 y, color_t clr);
void Draw4PixelsRaw(frame_t& frame, u32 x, u32 y, color_t clr);
color_t FetchPixelColor(frame_t& frame, uint32_t x, uint32_t y);

constexpr color_t MakeColor(u8 r, u8 g, u8 b, u8 a)
{
    color_t clr { 0 };
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = a;
    return clr;
}

constexpr u8 BlendColor(u32 src, u32 dst, u8 alpha)
{
    u8 one_minus_alpha = (u8)255 - alpha;
    return (dst*alpha + src*one_minus_alpha)/(u8)255;
}
