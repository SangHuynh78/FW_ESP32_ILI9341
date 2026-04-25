set(srcs "fonts.c"
         "ili9341_gpio.c" 
         "ili9341_spi.c"
         "ili9341.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "."
                       REQUIRES driver)