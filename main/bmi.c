/*
 *   Source file for bmi methods for reading, writing, initializing
 */
#include "bmi.h"
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

uint8_t read_buf[2];

/* BMI160_Init - Excecutes upon init of the FD system ensuring power-ons and configurations */

esp_err_t BMI160_Init(i2c_master_dev_handle_t dev_handle) {
    uint8_t init_sequence[][2] = {{CMD, 0x11}, {CMD, 0x15}, {ACC_RANGE, 0x03},{GYR_RANGE, 0x03}}; //Accel: ±2g, Gyro: ±250°/s
    
    for(int i=0; i < 4; i++) {
        BMI160_WriteRegister(dev_handle, init_sequence[i][0], init_sequence[i][1]);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

/* BMI160_WriteRegister - Method for writing data to a specific adress on the BMI*/

esp_err_t BMI160_WriteRegister(i2c_master_dev_handle_t dev_handle, uint8_t regg_adress, uint8_t data_buf)
{
    uint8_t write_buf[2] = {regg_adress, data_buf};
    return (i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS));
}

/* BMI160_ReadRegister - Method for reading data from a specific adress on the BMI to a read buffer */

esp_err_t BMI160_ReadRegister(i2c_master_dev_handle_t dev_handle, uint8_t regg_adress, uint8_t * data_buf, size_t len){
    return i2c_master_transmit_receive(dev_handle, &regg_adress, 1, data_buf, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/* BMI160_ReadAccel - Method for reading accelerometer data across a specified axis */

int16_t BMI160_ReadAccel(i2c_master_dev_handle_t dev_handle, Axis axis){
    int16_t axis_value = 0;

    switch(axis){
        case AXIS_X:
            BMI160_ReadRegister(dev_handle, ACC_MSB_X, &read_buf[0], 1);
            BMI160_ReadRegister(dev_handle, ACC_LSB_X, &read_buf[1], 1);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Y:
            BMI160_ReadRegister(dev_handle, ACC_MSB_Y, &read_buf[0], 1);
            BMI160_ReadRegister(dev_handle, ACC_LSB_Y, &read_buf[1], 1);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Z:
            BMI160_ReadRegister(dev_handle, ACC_MSB_Z, &read_buf[0], 1);
            BMI160_ReadRegister(dev_handle, ACC_LSB_Z, &read_buf[1], 1);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;
    }
    return axis_value;
}

/* BMI160_ReadGyro - Method for reading accelerometer data across a specified axis */

int16_t BMI160_ReadGyro(i2c_master_dev_handle_t dev_handle, Axis axis){
    int16_t axis_value = 0;

    switch(axis){
        case AXIS_X:
            BMI160_ReadRegister(dev_handle, GYR_MSB_X, &read_buf[0], 1);
            BMI160_ReadRegister(dev_handle, GYR_LSB_X, &read_buf[1], 1);
            axis_value = ((int16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Y:
            BMI160_ReadRegister(dev_handle, GYR_MSB_Y, &read_buf[0], 1);
            BMI160_ReadRegister(dev_handle, GYR_LSB_Y, &read_buf[1], 1);
            axis_value = ((int16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Z:
            BMI160_ReadRegister(dev_handle, GYR_MSB_Z, &read_buf[0], 1);
            BMI160_ReadRegister(dev_handle, GYR_LSB_Z, &read_buf[1], 1);
            axis_value = ((int16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;
    }
    return axis_value;
}