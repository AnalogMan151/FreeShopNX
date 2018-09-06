#include "color.hpp"
#include "globals.hpp"

void DrawPixel(frame_t& frame, u32 x, u32 y, color_t clr)
{
    if (x >= 1280 || y >= 720)
        return;
    u32 off = (y * frame.width + x)*4;
    frame.buffer[off] = BlendColor(frame.buffer[off], clr.r, clr.a); off++;
    frame.buffer[off] = BlendColor(frame.buffer[off], clr.g, clr.a); off++;
    frame.buffer[off] = BlendColor(frame.buffer[off], clr.b, clr.a); off++;
    frame.buffer[off] = 0xff;
}

void DrawPixelRaw(frame_t& frame, u32 x, u32 y, color_t clr)
{
    if (x >= 1280 || y >= 720)
        return;
    u32 off = (y * frame.width + x)*4;
    *((u32*)&frame.buffer[off]) = clr.r | (clr.g<<8) | (clr.b<<16) | (0xff<<24);
}

void Draw4PixelsRaw(frame_t& frame, u32 x, u32 y, color_t clr)
{
    if (x >= 1280 || y >= 720 || x > 1280-4)
        return;

    u32 color = clr.r | (clr.g<<8) | (clr.b<<16) | (0xff<<24);
    u128 val = color | ((u128)color<<32) | ((u128)color<<64) | ((u128)color<<96);
    u32 off = (y * frame.width + x)*4;
    *((u128*)&frame.buffer[off]) = val;
}

color_t FetchPixelColor(frame_t& frame, uint32_t x, uint32_t y)
{
    u32 off = (y * frame.width + x)*4;
    u32 val = *((u32*)&frame.buffer[off]);
    u8 r = (u8)val;
    u8 g = (u8)(val>>8);
    u8 b = (u8)(val>>16);
    return MakeColor(r, g, b, 255);
}