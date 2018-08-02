#pragma once

typedef struct {
    uint8_t magic[4]; // 'fFNT'
    int version;  // 1
    uint16_t npages;
    uint8_t height;
    uint8_t baseline;
} ffnt_header_t;

typedef struct {
    uint32_t size, offset;
} ffnt_pageentry_t;

typedef struct {
    uint32_t pos[0x100];
    uint8_t widths[0x100];
    uint8_t heights[0x100];
    int8_t advances[0x100];
    int8_t posX[0x100];
    int8_t posY[0x100];
} ffnt_pagehdr_t;

typedef struct {
    ffnt_pagehdr_t hdr;
    uint8_t data[];
} ffnt_page_t;

typedef struct {
    uint8_t width, height;
    int8_t posX, posY, advance, pitch;
    const uint8_t* data;
} glyph_t;

struct coord {
    uint32_t x, y;
};

#define fontHuge 10
#define fontLarge 8
#define fontMedium 6
#define fontSmall 4
#define fontTiny 3

#define FON_A "\uE0E0"
#define FON_B "\uE0E1"
#define FON_X "\uE0E2"
#define FON_Y "\uE0E3"
#define FON_L "\uE0E4"
#define FON_R "\uE0E5"
#define FON_ZL "\uE0E6"
#define FON_ZR "\uE0E7"
#define FON_SL "\uE0E8"
#define FON_SR "\uE0E9"
#define FON_UP "\uE0EB"
#define FON_DN "\uE0EC"
#define FON_LT "\uE0ED"
#define FON_RT "\uE0EE"
#define FON_PL "\uE0EF"
#define FON_MI "\uE0F0"
#define FON_HM "\uE0F4"
#define FON_SS "\uE0F5"
#define FON_LS "\uE101"
#define FON_RS "\uE102"
#define FON_L3 "\uE104"
#define FON_R3 "\uE105"