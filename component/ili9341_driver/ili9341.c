#include "ili9341.h"
#include "ili9341_setup.h"
#include "ili9341_gpio.h"
#include "ili9341_spi.h"
#include <stdint.h>

static void ili9341_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ili9341_spi_write_cmd(0x2A); // CASET
    {
        uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // row address set
    ili9341_spi_write_cmd(0x2B); // RASET
    {
        uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // write to RAM
    ili9341_spi_write_cmd(0x2C); // RAMWR
}

static void ili9341_init_driver() {
    ili9341_select();
    ili9341_gpio_reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

    // SOFTWARE RESET
    ili9341_spi_write_cmd(0x01);
    esp_rom_delay_us(1000000);
        
    // POWER CONTROL A
    ili9341_spi_write_cmd(0xCB);
    {
        uint8_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // POWER CONTROL B
    ili9341_spi_write_cmd(0xCF);
    {
        uint8_t data[] = { 0x00, 0xC1, 0x30 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    ili9341_spi_write_cmd(0xE8);
    {
        uint8_t data[] = { 0x85, 0x00, 0x78 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    ili9341_spi_write_cmd(0xEA);
    {
        uint8_t data[] = { 0x00, 0x00 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    ili9341_spi_write_cmd(0xED);
    {
        uint8_t data[] = { 0x64, 0x03, 0x12, 0x81 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    ili9341_spi_write_cmd(0xF7);
    {
        uint8_t data[] = { 0x20 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    ili9341_spi_write_cmd(0xC0);
    {
        uint8_t data[] = { 0x23 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ili9341_spi_write_cmd(0xC1);
    {
        uint8_t data[] = { 0x10 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // VCM CONTROL
    ili9341_spi_write_cmd(0xC5);
    {
        uint8_t data[] = { 0x3E, 0x28 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // VCM CONTROL 2
    ili9341_spi_write_cmd(0xC7);
    {
        uint8_t data[] = { 0x86 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    ili9341_spi_write_cmd(0x36);
    {
        uint8_t data[] = { 0x48 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // PIXEL FORMAT
    ili9341_spi_write_cmd(0x3A);
    {
        uint8_t data[] = { 0x55 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ili9341_spi_write_cmd(0xB1);
    {
        uint8_t data[] = { 0x00, 0x18 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    ili9341_spi_write_cmd(0xB6);
    {
        uint8_t data[] = { 0x08, 0x82, 0x27 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    ili9341_spi_write_cmd(0xF2);
    {
        uint8_t data[] = { 0x00 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    ili9341_spi_write_cmd(0x26);
    {
        uint8_t data[] = { 0x01 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    ili9341_spi_write_cmd(0xE0);
    {
        uint8_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    ili9341_spi_write_cmd(0xE1);
    {
        uint8_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        ili9341_spi_write_data(data, sizeof(data));
    }

    // EXIT SLEEP
    ili9341_spi_write_cmd(0x11);
    esp_rom_delay_us(120000);

    // TURN ON DISPLAY
    ili9341_spi_write_cmd(0x29);

    // MADCTL
    ili9341_spi_write_cmd(0x36);
    {
        uint8_t data[] = { ILI9341_ROTATION };
        ili9341_spi_write_data(data, sizeof(data));
    }

    ili9341_unselect();
}

static void ili9341_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ili9341_set_address_window(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ili9341_spi_write_data(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ili9341_spi_write_data(data, sizeof(data));
            }
        }
    }
}

void ili9341_init() {
    ili9341_gpio_init();
    ili9341_spi_init();
    ili9341_gpio_reset();

    ili9341_init_driver();
    ili9341_fill_screen(ILI9341_BLACK);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
        return;

    ili9341_select();

    ili9341_set_address_window(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ili9341_spi_write_data(data, sizeof(data));

    ili9341_unselect();
}

void ili9341_write_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    ili9341_select();

    while(*str) {
        if(x + font.width >= ILI9341_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= ILI9341_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ili9341_write_char(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }

    ili9341_unselect();
}

void ili9341_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    uint32_t total_pixels = (uint32_t)w * h;

    ili9341_select();
    ili9341_set_address_window(x, y, x + w - 1, y + h - 1);

    ili9341_spi_write_color(color, total_pixels);
}

void ili9341_fill_screen(uint16_t color) {
    ili9341_fill_rectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ili9341_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) return;
    if((y + h - 1) >= ILI9341_HEIGHT) return;

    ili9341_select();
    ili9341_set_address_window(x, y, x+w-1, y+h-1);
    ili9341_spi_write_data((uint8_t*)data, sizeof(uint16_t)*w*h);
    ili9341_unselect();
}

void ili9341_invert_colors(bool invert) {
    ili9341_select();
    ili9341_spi_write_cmd(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
    ili9341_unselect();
}