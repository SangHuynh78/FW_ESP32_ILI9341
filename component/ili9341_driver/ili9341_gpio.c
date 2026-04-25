#include "ili9341_gpio.h"
#include "ili9341_setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "ILI9341_GPIO";

void ili9341_gpio_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << TFT_DC) | (1ULL << TFT_RST) | (1ULL << TFT_CS),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    gpio_set_level(TFT_CS, 1);
    gpio_set_level(TFT_RST, 1);
    gpio_set_level(TFT_DC, 1);
}

void ili9341_gpio_reset(void) {
    ESP_LOGI(TAG, "Performing Hard Reset...");
    
    gpio_set_level(TFT_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    
    gpio_set_level(TFT_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(150));
}

void ili9341_gpio_set_dc(uint8_t level)
{
    gpio_set_level(TFT_DC, level);
}

void ili9341_select(void) {
    gpio_set_level(TFT_CS, 0);
}

void ili9341_unselect(void) {
    gpio_set_level(TFT_CS, 1);
}