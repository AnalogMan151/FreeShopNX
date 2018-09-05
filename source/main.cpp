#include "common.hpp"
#include "globals.hpp"

int main(int argc, char **argv)
{
    g_scene = &title_scene;
    Result rc = 0;
    g_infoLoaded = loadInfo();
    g_titlesLoaded = loadTitles();

    gfxInitDefault();
    socketInitializeDefault();

    appletSetScreenShotPermission(1);

    ColorSetId theme;
    rc = setsysInitialize();

    if (R_FAILED(ncmInitialize()))
        fatalSimple(-1);

    if (R_FAILED(nsInitialize()))
        fatalSimple(-2);
    
    if (R_FAILED(nsextInitialize()))
        fatalSimple(-3);

    if (R_FAILED(esInitialize()))
        fatalSimple(-4);

    if (R_FAILED(rc))
        fatalSimple(-5);

    setsysGetColorSetId(&theme);
    themeStartup((ThemePreset)theme);

    rc = plInitialize();
    if (R_FAILED(rc))
        fatalSimple(-6);

    if (!fontInitialize())
        fatalSimple(-7);

    while (appletMainLoop())
    {
        g_framebuf = gfxGetFramebuffer(&g_framebuf_width, NULL);
        memset(g_framebuf, 237, gfxGetFramebufferSize());

        drawUI();
        g_scene->draw();

        gfxFlushBuffers();

        gfxSwapBuffers();
        gfxWaitForVsync();

        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS) break; // Handle the exit key whatever the scene
        g_scene->handle_input(kDown, kHeld);
    }

    socketExit();
    fontExit();
    plExit();
    esExit();
    nsextExit();
    nsExit();
    ncmExit();
    setsysExit();

    gfxExit();
    return 0;
}