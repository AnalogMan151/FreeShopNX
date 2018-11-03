extern "C" {
    #include <switch/display/gfx.h>
    #include <switch/runtime/devices/socket.h>
    #include <switch/services/applet.h>
    #include <switch/services/fatal.h>
    #include <switch/services/hid.h>
    #include <switch/services/pl.h>
    #include <switch/services/set.h>
    #include <switch/services/ncm.h>
    #include <switch/services/ns.h>
    #include <switch/result.h>
}
#include "globals.hpp"
#include "font.hpp"
#include "install.hpp"
#include "ui.hpp"

int main(int argc, char **argv)
{
    appletLockExit();
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
        frame_t frame;
        frame.buffer = gfxGetFramebuffer(&frame.width, &frame.height);
        memset(frame.buffer, 237, gfxGetFramebufferSize());

        drawUI(frame);
        g_scene->draw(frame);

        gfxFlushBuffers();

        gfxSwapBuffers();
        gfxWaitForVsync();

        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
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
    appletUnlockExit();
    return 0;
}