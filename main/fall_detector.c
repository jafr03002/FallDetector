/*
 *  Fall Detection thesis project with the use of BMI160 and the ESP32 mcu
 *  
 *  fall_detector.c:
 *  
 *  Initializes and configures the i2c busses
 *  Retrieves and calls bmi functions through bmi.h 
 *  Calculating the raw data into accel: m/s^2 and gyr: deg/s
 *  Value printouts
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

/* Stores raw values of every axis*/

int16_t ACC_X = 0;
int16_t ACC_Y = 0;
int16_t ACC_Z = 0;

int16_t GYR_X = 0;
int16_t GYR_Y = 0;
int16_t GYR_Z = 0;


/* Task handler for referencing */

TaskHandle_t BMI180ReadHandle = NULL;

float g = 9.81;

/* Actual metrics variable placeholders for printouts */

float ax_real = 0;
float ay_real = 0;
float az_real = 0;

float ax_m_per_s = 0;
float ay_m_per_s = 0;
float az_m_per_s = 0;

float gx_real = 0;
float gy_real = 0;
float gz_real = 0;

/* Master i2c bus init and config */

static void i2c_master_init_bus(i2c_master_bus_handle_t *bus_handle){
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_1,
        .scl_io_num = SCL,
        .sda_io_num = SDA,
        .flags.enable_internal_pullup = true,
        .glitch_ignore_cnt = 7,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, bus_handle)); /* the i2c master bus handle, referal for the R/W functions */
}

/* Master i2c bus handle init and dev configuration */

static void i2c_master_init_handle(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle, uint8_t address){
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = 100000,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle)); // device handler
}

/* 
 * detector_task - 100Hz IMU values retriever
 * FD main loop - initializes i2c master bus and dev handle
 * Reads BMI160 gyro and accelerometer values across 6-axises
 * Caulculate real world metrics from raw data
 * Prints real values
 * Repeat after 10ms
 */
 

void detector_task(void *arg){
    /* i2c init */
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    i2c_master_init_bus(&bus_handle);

    i2c_master_init_handle(&bus_handle, &dev_handle, DEV_ADDR);
    BMI160_Init(dev_handle);
    while(1){
        ACC_X = BMI160_ReadAccel(dev_handle, AXIS_X);
        ACC_Y = BMI160_ReadAccel(dev_handle, AXIS_Y);
        ACC_Z = BMI160_ReadAccel(dev_handle, AXIS_Z);

        GYR_X = BMI160_ReadGyro(dev_handle, AXIS_X);
        GYR_Y = BMI160_ReadGyro(dev_handle, AXIS_Y);
        GYR_Z = BMI160_ReadGyro(dev_handle, AXIS_Z);
        
        /* Calculate real accel and gyr values */
        
        ax_real = (float)ACC_X/16384.0;
        ay_real = (float)ACC_Y/16384.0;
        az_real = (float)ACC_Z/16384.0;

        ax_m_per_s = ax_real*9.81;
        ay_m_per_s = ay_real*9.81;
        az_m_per_s = az_real*9.81;

        gx_real = (float)GYR_X/131.0;
        gy_real = (float)GYR_Y/131.0;
        gz_real = (float)GYR_Z/131.0;

        printf("%0.2f %0.2f %0.2f %0.1f %0.1f %0.1f\n", ax_m_per_s, ay_m_per_s, az_m_per_s, gx_real, gy_real, gz_real);
        fflush(stdout);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(detector_task, "detector", 4096, NULL, 10, &BMI180ReadHandle, 1); 
}
