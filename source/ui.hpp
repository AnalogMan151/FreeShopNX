#pragma once

#include <switch/types.h>

struct frame_t
{
    u8* buffer;
    u32 width;
    u32 height;
};

void drawSeperators(frame_t& frame);
void drawUI(frame_t& frame);