#include "common.hpp"
#include "globals.hpp"

void drawSeperators(void)
{
    for (int x = 30; x < 1250; x++)
    {
        DrawPixel(x, 54, themeCurrent.seperatorColor);
        DrawPixel(x, 676, themeCurrent.seperatorColor);
    }
}

void drawUI(void)
{
    for (int y = 0; y < 720; y++)
    {
        for (int x = 0; x < 1280; x += 4)
        {
            Draw4PixelsRaw(x, y, themeCurrent.backgroundColor);
        }
    }

    struct coord pos = {30, 32};
    pos = DrawText(fontHuge, pos.x, 40, themeCurrent.textColor, "FreeShop");
    pos = DrawText(fontHuge, pos.x, 40, MakeColor(230, 0, 18, 255), "N");
    pos = DrawText(fontHuge, pos.x, 40, themeCurrent.textColor, "e");
    pos = DrawText(fontHuge, pos.x, 40, MakeColor(230, 0, 18, 255), "X");
    pos = DrawText(fontHuge, pos.x, 40, themeCurrent.textColor, "t");
}