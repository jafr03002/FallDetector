idf_component_register(SRCS "bmi.c" "fall_detecter.c"
                    PRIV_REQUIRES spi_flash esp_driver_i2c
                    INCLUDE_DIRS ".")