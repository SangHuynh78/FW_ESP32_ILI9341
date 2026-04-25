#include "ili9341_spi.h"
#include "ili9341_setup.h"
#include "ili9341_gpio.h"
#include "driver/spi_master.h"
#include <string.h>

spi_device_handle_t tft_spi;
spi_device_handle_t touch_spi;

void ili9341_spi_init(void)
{
    spi_bus_config_t buscfg = {
        .miso_io_num = TFT_MISO,
        .mosi_io_num = TFT_MOSI,
        .sclk_io_num = TFT_SCLK,
        .max_transfer_sz = 4096,
    };

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // TFT (40 MHz)
    spi_device_interface_config_t devcfg_tft = {
        .clock_speed_hz = SPI_FREQUENCY,
        .mode = 0,
        .spics_io_num = TFT_CS,
        .queue_size = 7,
    };

    spi_bus_add_device(SPI2_HOST, &devcfg_tft, &tft_spi);

    // TOUCH (2.5 MHz)
    // spi_device_interface_config_t devcfg_touch = {
    //     .clock_speed_hz = SPI_TOUCH_FREQUENCY,
    //     .mode = 0,
    //     .spics_io_num = TOUCH_CS,
    //     .queue_size = 3,
    // };

    // spi_bus_add_device(SPI2_HOST, &devcfg_touch, &touch_spi);

    // IRQ pin
    // gpio_set_direction(TOUCH_IRQ, GPIO_MODE_INPUT);
}

void ili9341_spi_write_cmd(uint8_t cmd)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = 8;
    t.flags = SPI_TRANS_USE_TXDATA;
    t.tx_data[0] = cmd;

    ILI9341_DC_COMMAND();
    spi_device_transmit(tft_spi, &t);
}

void ili9341_spi_write_data(const uint8_t *data, int len)
{
    if (len <= 0) return;

    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = data,
    };

    ILI9341_DC_DATA();

    ESP_ERROR_CHECK(spi_device_transmit(tft_spi, &t));
}

void ili9341_spi_write_data16(uint16_t data)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t buf[2] = {
        data >> 8,
        data & 0xFF
    };

    t.length = 16;
    t.tx_buffer = buf;

    ILI9341_DC_DATA();
    spi_device_transmit(tft_spi, &t);
}

void ili9341_spi_write_buffer(const uint8_t *data, int len)
{
    if (len == 0) return;

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = len * 8;
    t.tx_buffer = data;

    ILI9341_DC_DATA();
    spi_device_transmit(tft_spi, &t);
}

void ili9341_spi_write_color(uint16_t color, int len)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    const int chunk_size = 64;
    uint8_t buffer[chunk_size * 2];

    for (int i = 0; i < chunk_size; i++) {
        buffer[2*i] = hi;
        buffer[2*i + 1] = lo;
    }

    ILI9341_DC_DATA();

    while (len > 0) {
        int send = (len > chunk_size) ? chunk_size : len;

        spi_transaction_t t;
        memset(&t, 0, sizeof(t));

        t.length = send * 16;
        t.tx_buffer = buffer;

        spi_device_transmit(tft_spi, &t);

        len -= send;
    }
}