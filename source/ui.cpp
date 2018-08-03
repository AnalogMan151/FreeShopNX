#include "common.hpp"

int installLocation = 0;

void drawUI(void)
{
    for (int y = 0; y < 720; y++)
    {
        for (int x = 0; x < 1280; x += 4)
        {
            Draw4PixelsRaw(x, y, themeCurrent.backgroundColor);
        }
    }
    for (int x = 30; x < 1250; x++)
    {
        DrawPixel(x, 54, themeCurrent.seperatorColor);
        DrawPixel(x, 676, themeCurrent.seperatorColor);
    }

    struct coord pos = {30, 32};
    pos = DrawText(fontHuge, pos.x, 40, themeCurrent.textColor, "FreeShop");
    pos = DrawText(fontHuge, pos.x, 40, MakeColor(230, 0, 18, 255), "N");
    pos = DrawText(fontHuge, pos.x, 40, themeCurrent.textColor, "e");
    pos = DrawText(fontHuge, pos.x, 40, MakeColor(230, 0, 18, 255), "X");
    pos = DrawText(fontHuge, pos.x, 40, themeCurrent.textColor, "t");

    uint32_t rightX1;
    uint32_t rightX2;
    char btnConfig_top1[64];
    char btnConfig_top2[64];
    sprintf(btnConfig_top1, FON_X " %s", (installLocation) ? "NAND" : "SD");
    sprintf(btnConfig_top2, FON_Y " Update List");
    GetTextDimensions(fontSmall, FON_X " NAND   " FON_Y " Update List", &rightX1, NULL);
    GetTextDimensions(fontSmall, btnConfig_top2, &rightX2, NULL);
    DrawText(fontSmall, 1280 - rightX1 - 30, 32, themeCurrent.textColor, btnConfig_top1);
    DrawText(fontSmall, 1280 - rightX2 - 30, 32, themeCurrent.textColor, btnConfig_top2);
}