#include "ili9341.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "ILI9341_MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "ILI9341 init...");

    ili9341_init();

    ESP_LOGI(TAG, "LCD initialized");
    while (1)
    {
        ESP_LOGI(TAG, "Color cycle...");

        ili9341_fill_screen(ILI9341_RED);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ili9341_fill_screen(ILI9341_GREEN);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ili9341_fill_screen(ILI9341_BLUE);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ili9341_fill_screen(ILI9341_BLACK);
        ili9341_write_string(10, 10, "Hello ESP32", Font_11x18, ILI9341_WHITE, ILI9341_BLACK);
        ili9341_write_string(10, 40, "ILI9341 OK", Font_11x18, ILI9341_GREEN, ILI9341_BLACK);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}