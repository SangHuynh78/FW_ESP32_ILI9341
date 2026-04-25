#ifndef ILI9341_H
#define ILI9341_H

#include "fonts.h"
#include <stdbool.h>

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

/*** Redefine if necessary ***/

// default orientation
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MV | ILI9341_MADCTL_RGB)

// rotate right
// #define ILI9341_WIDTH  320
// #define ILI9341_HEIGHT 240
// #define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_RGB)

// rotate left
// #define ILI9341_WIDTH  320
// #define ILI9341_HEIGHT 240
// #define ILI9341_ROTATION (ILI9341_MADCTL_RGB)

// upside down
// #define ILI9341_WIDTH  240
// #define ILI9341_HEIGHT 320
// #define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_RGB)

/****************************/

// Color definitions
#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0x001F
#define	ILI9341_RED     0xF800
#define	ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF
#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

void ili9341_init(void);
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9341_write_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ili9341_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_fill_screen(uint16_t color);
void ili9341_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void ili9341_invert_colors(bool invert);

#endif // ILI9341_H