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
#include "driver/i2c_slave.h"
#include "bmi.h"

#define SDA 21
#define SCL 22

int16_t ACC_X = 0;
int16_t ACC_Y = 0;
int16_t ACC_Z = 0;

int16_t GYR_X = 0;
int16_t GYR_Y = 0;
int16_t GYR_Z = 0;

TaskHandle_t BMI180ReadHandle = NULL;
TaskHandle_t CheckAddressHandle = NULL;

uint8_t who_buf[1];

float g = 9.81;

float ax_real = 0;
float ay_real = 0;
float az_real = 0;

float ax_m_per_s = 0;
float ay_m_per_s = 0;
float az_m_per_s = 0;

float gx_real = 0;
float gy_real = 0;
float gz_real = 0;

static void i2c_master_init_bus(i2c_master_bus_handle_t *bus_handle){
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_1,
        .scl_io_num = SCL,
        .sda_io_num = SDA,
        .flags.enable_internal_pullup = true,
        .glitch_ignore_cnt = 7,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, bus_handle));
}

static void i2c_master_init_handle(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle, uint8_t address){
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = 100000,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
}

void check_address_task(void *arg){
    i2c_master_bus_handle_t *bus_handle = (i2c_master_bus_handle_t*) arg;
    while(1){
        esp_err_t err;
        // Using i2c_master_probe on a for loop going through all available address
        for(uint8_t i = 3; i < 0x78; i++){
            err = i2c_master_probe(*bus_handle, i, 1000);
            if (err == ESP_OK){
                printf("I2C Scanner found I2C device at: 0x%X \n", i);
            }
        }
        printf("I2C_Scanner complete \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void detector_task(void *arg){
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    i2c_master_init_bus(&bus_handle);

    i2c_master_init_handle(&bus_handle, &dev_handle, DEV_ADDR);

    while(1){
        uint8_t wakeup_data = 0x00;
        BMI160_WriteRegister(dev_handle, MPU_PWR_MGMT, wakeup_data);

        ACC_X = MPU_ReadAccel(dev_handle, AXIS_X);
        ACC_Y = MPU_ReadAccel(dev_handle, AXIS_Y);
        ACC_Z = MPU_ReadAccel(dev_handle, AXIS_Z);

        GYR_X = MPU_ReadGyro(dev_handle, AXIS_X);
        GYR_Y = MPU_ReadGyro(dev_handle, AXIS_Y);
        GYR_Z = MPU_ReadGyro(dev_handle, AXIS_Z);

        //printf("first A: %d, %d, %d G: %d, %d, %d\n", ACC_X, ACC_Y, ACC_Z, GYR_X, GYR_Y, GYR_Z);

        ax_real = (float)ACC_X/16384.0;
        ay_real = (float)ACC_Y/16384.0;
        az_real = (float)ACC_Z/16384.0;

        ax_m_per_s = ax_real*9.81;
        ay_m_per_s = ay_real*9.81;
        az_m_per_s = az_real*9.81;

        gx_real = (float)GYR_X/131.0;
        gy_real = (float)GYR_Y/131.0;
        gz_real = (float)GYR_Z/131.0;

        printf("Accel(m/s^2): X: %0.2f, Y: %0.2f, Z: %0.2f Gyro(dps): X: %0.1f, Y: %0.1f, Z: %0.1f\n", ax_m_per_s, ay_m_per_s, az_m_per_s, gx_real, gy_real, gz_real);
    
        vTaskDelay(1000/portTICK_PERIOD_MS); 
    }

}

void app_main(void)
{
    xTaskCreatePinnedToCore(detector_task, "detector", 4096, NULL, 10, &BMI180ReadHandle, 1); 
}
