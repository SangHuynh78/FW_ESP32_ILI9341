#ifndef ILI9341_GPIO_H
#define ILI9341_GPIO_H

#include "driver/gpio.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ILI9341_DC_COMMAND() ili9341_gpio_set_dc(0)
#define ILI9341_DC_DATA()    ili9341_gpio_set_dc(1)

void ili9341_gpio_init(void);
void ili9341_gpio_reset(void);
void ili9341_gpio_set_dc(uint8_t level);

void ili9341_select(void);
void ili9341_unselect(void);

#ifdef __cplusplus
}
#endif

#endif // ILI9341_GPIO_H