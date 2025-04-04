/*
 *  Fall detector main file 
 *
 *  Author: Jakob Freij
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "driver/i2c_master.h"
#include "bmi.h"

#define SDA 22
#define SCL 21

float ACC_X = 0;
float ACC_Y = 0;
float ACC_Z = 0;
float GYR_X = 0;
float GYR_Y = 0;
float GYR_Z = 0;

void detector_task(void *args){

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = SCL,
        .sda_io_num = SDA,
        .flags.enable_internal_pullup = true,
        .glitch_ignore_cnt = 7,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));


    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DEV_ADDR,
        .scl_speed_hz = 100000,
    };
    
    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

    BMI160_Init(dev_handle);

    while(1){
        ACC_X = BMI160_ReadAccel(dev_handle, AXIS_X);
        ACC_Y = BMI160_ReadAccel(dev_handle, AXIS_Y);
        ACC_Z = BMI160_ReadAccel(dev_handle, AXIS_Z);

        GYR_X = BMI160_ReadGyro(dev_handle, AXIS_X);
        GYR_Y = BMI160_ReadGyro(dev_handle, AXIS_Y);
        GYR_Z = BMI160_ReadGyro(dev_handle, AXIS_Z);

        BMI160_PrintValues();
    }
}

void app_main(void)
{
    xTaskCreate(&detector_task, "detector", 2048, NULL, 5, NULL); 
}
