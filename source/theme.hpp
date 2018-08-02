#pragma once

typedef struct
{
    color_t textColor;
    color_t selectedColor;
    color_t backgroundColor;
    color_t highlightColor;
    color_t seperatorColor;
} theme_t;

typedef enum
{
    THEME_PRESET_LIGHT,
    THEME_PRESET_DARK,
} ThemePreset;

void themeStartup(ThemePreset preset);

extern theme_t themeCurrent;
