#pragma once

#define TITLE_SCENE 0
#define INFO_SCENE 1
#define UPDATE_SCENE 2
#define ABOUT_SCENE 3
#define INSTALL_SCENE 4

extern int g_scene;

void titleScene(void);
void infoScene(void);
void updateScene(void);
void aboutScene(void);
void installScene(void);
void buttonA(void);
void buttonB(void);
void buttonX(void);
void buttonY(void);
void buttonMinus(void);
void buttonUpDown(void);
void buttonLeftRight(void);
void buttonLR(void);