#ifndef ILI9341_SPI_H
#define ILI9341_SPI_H

#include "driver/spi_master.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern spi_device_handle_t tft_spi;
extern spi_device_handle_t touch_spi;

void ili9341_spi_init(void);
void ili9341_spi_write_cmd(uint8_t cmd);
void ili9341_spi_write_data(const uint8_t *data, int len);

void ili9341_spi_write_data16(uint16_t data);
void ili9341_spi_write_buffer(const uint8_t *data, int len);
void ili9341_spi_write_color(uint16_t color, int len);

#ifdef __cplusplus
}
#endif

#endif // ILI9341_SPI_H