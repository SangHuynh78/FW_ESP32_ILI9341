#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Định nghĩa chân GPIO
#define LED_1_IO    4
#define LED_2_IO    16

static const char *TAG = "LED_TEST";

// Task 1: Nháy LED 1 mỗi 500ms
void led_task_1(void *pvParameters) {
    // Cấu hình chân GPIO
    gpio_reset_pin(LED_1_IO);
    gpio_set_direction(LED_1_IO, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(LED_1_IO, 1);
        vTaskDelay(pdMS_TO_TICKS(500)); // Nghỉ 500ms
        gpio_set_level(LED_1_IO, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        ESP_LOGI(TAG, "LED 1 (GPIO 4) vùa nháy!");
    }
}

// Task 2: Nháy LED 2 mỗi 1000ms (chạy chậm hơn task 1)
void led_task_2(void *pvParameters) {
    gpio_reset_pin(LED_2_IO);
    gpio_set_direction(LED_2_IO, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(LED_2_IO, 1);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Nghỉ 1s
        gpio_set_level(LED_2_IO, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "LED 2 (GPIO 16) vừa nháy!");
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Bat dau test FreeRTOS với 2 LED...");

    // Tạo Task 1 nháy LED 1 - Ưu tiên mức 5, chạy trên Nhân 0
    xTaskCreatePinnedToCore(led_task_1, "led_task_1", 2048, NULL, 5, NULL, 0);

    // Tạo Task 2 nháy LED 2 - Ưu tiên mức 5, chạy trên Nhân 1
    xTaskCreatePinnedToCore(led_task_2, "led_task_2", 2048, NULL, 5, NULL, 1);
    
    ESP_LOGI(TAG, "2 Task da duoc tao thành công!");
}