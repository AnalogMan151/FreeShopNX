#include "common.hpp"
#include "globals.hpp"

void themeStartup(ThemePreset preset) {
    switch (preset) {
        case THEME_PRESET_LIGHT:
            themeCurrent = (theme_t){
                .textColor = MakeColor(0, 0, 0, 255),
                .selectedColor = MakeColor(0, 195, 227, 255),
                .backgroundColor = MakeColor(233, 236, 241, 255),
                .backgroundDim = MakeColor(0, 0, 0, 192),
                .highlightColor = MakeColor(223, 226, 231, 255),
                .seperatorColor = MakeColor(0, 0, 0, 255),
            };
            break;

        case THEME_PRESET_DARK:
            themeCurrent = (theme_t){
                .textColor = MakeColor(255, 255, 255, 255),
                .selectedColor = MakeColor(255, 69, 84, 255),
                .backgroundColor = MakeColor(45, 45, 50, 255),
                .backgroundDim = MakeColor(0, 0, 0, 192),
                .highlightColor = MakeColor(25, 25, 30, 255),
                .seperatorColor = MakeColor(219, 218, 219, 255),
            };
            break;
    }
}