#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <switch.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u32 Result;

typedef union {
    uint32_t abgr;
    struct
    {
        uint8_t r, g, b, a;
    };
} color_t;

#include "font.hpp"
#include "theme.hpp"
#include "ui.hpp"
#include "scenes.hpp"

static inline uint8_t BlendColor(uint32_t src, uint32_t dst, uint8_t alpha)
{
    uint8_t one_minus_alpha = (uint8_t)255 - alpha;
    return (dst*alpha + src*one_minus_alpha)/(uint8_t)255;
}

static inline color_t MakeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    color_t clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = a;
    return clr;
}

extern uint8_t* g_framebuf;
extern u32 g_framebuf_width;
static inline void DrawPixel(uint32_t x, uint32_t y, color_t clr)
{
    if (x >= 1280 || y >= 720)
        return;
    u32 off = (y * g_framebuf_width + x)*4;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.r, clr.a); off++;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.g, clr.a); off++;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.b, clr.a); off++;
    g_framebuf[off] = 0xff;
}
static inline void DrawPixelRaw(uint32_t x, uint32_t y, color_t clr)
{
    if (x >= 1280 || y >= 720)
        return;
    u32 off = (y * g_framebuf_width + x)*4;
    *((u32*)&g_framebuf[off]) = clr.r | (clr.g<<8) | (clr.b<<16) | (0xff<<24);
}
static inline void Draw4PixelsRaw(uint32_t x, uint32_t y, color_t clr)
{
    if (x >= 1280 || y >= 720 || x > 1280-4)
        return;

    u32 color = clr.r | (clr.g<<8) | (clr.b<<16) | (0xff<<24);
    u128 val = color | ((u128)color<<32) | ((u128)color<<64) | ((u128)color<<96);
    u32 off = (y * g_framebuf_width + x)*4;
    *((u128*)&g_framebuf[off]) = val;
}
static inline color_t FetchPixelColor(uint32_t x, uint32_t y)
{
    u32 off = (y * g_framebuf_width + x)*4;
    u32 val = *((u32*)&g_framebuf[off]);
    u8 r = (u8)val;
    u8 g = (u8)(val>>8);
    u8 b = (u8)(val>>16);
    return MakeColor(r, g, b, 255);
}

void DrawPixel(uint32_t x, uint32_t y, color_t clr);
struct coord DrawText(u32 font, uint32_t x, uint32_t y, color_t clr, const char* text);
struct coord DrawTextTruncate(u32 font, uint32_t x, uint32_t y, color_t clr, const char* text, uint32_t max_width, const char* end_text);
void GetTextDimensions(u32 font, const char* text, uint32_t* width_out, uint32_t* height_out);

bool fontInitialize(void);
void fontExit();