#include "ui.hpp"
extern "C" {
    #include <switch/display/gfx.h>
}
#include "globals.hpp"
#include "font.hpp"

void drawSeperators(frame_t& frame)
{
    for (int x = 30; x < 1250; x++)
    {
        DrawPixel(frame, x, 54, themeCurrent.seperatorColor);
        DrawPixel(frame, x, 676, themeCurrent.seperatorColor);
    }
}

void drawUI(frame_t& frame)
{
    for (int y = 0; y < 720; y++)
    {
        for (int x = 0; x < 1280; x += 4)
        {
            Draw4PixelsRaw(frame, x, y, themeCurrent.backgroundColor);
        }
    }

    struct coord pos = {30, 32};
    pos = DrawText(frame, fontHuge, pos.x, 40, themeCurrent.textColor, "FreeShop");
    pos = DrawText(frame, fontHuge, pos.x, 40, MakeColor(230, 0, 18, 255), "N");
    pos = DrawText(frame, fontHuge, pos.x, 40, themeCurrent.textColor, "e");
    pos = DrawText(frame, fontHuge, pos.x, 40, MakeColor(230, 0, 18, 255), "X");
    pos = DrawText(frame, fontHuge, pos.x, 40, themeCurrent.textColor, "t");
}