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

void BMI160_Init(i2c_master_dev_handle_t dev_handle)
{

    uint8_t initializing_buf[] = {ACC_CONF, ACC_RANGE, GYR_CONF, GYR_RANGE};
    for(int i=0; i < sizeof(initializing_buf); i++){
        BMI160_WriteRegister(dev_handle, CMD, initializing_buf[i]);
    }
}

void BMI160_WriteRegister(i2c_master_dev_handle_t dev_handle, uint8_t regg_adress, uint8_t data_buf)
{
    uint8_t write_buf[sizeof(data_buf)+1] = {regg_adress, data_buf};
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), -1));
}

void BMI160_ReadRegister(i2c_master_dev_handle_t dev_handle, uint8_t regg_adress, uint8_t * data_buf){
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &regg_adress, sizeof(regg_adress), data_buf, sizeof(data_buf), -1));
}

float BMI160_ReadAccel(i2c_master_dev_handle_t dev_handle, Axis axis){
    float axis_value = 0;
    switch(axis){
        case AXIS_X:
            BMI160_ReadRegister(dev_handle, ACC_MSB_X, &read_buf[0]);
            BMI160_ReadRegister(dev_handle, ACC_LSB_X, &read_buf[1]);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Y:
            BMI160_ReadRegister(dev_handle, ACC_MSB_Y, &read_buf[0]);
            BMI160_ReadRegister(dev_handle, ACC_LSB_Y, &read_buf[1]);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Z:
            BMI160_ReadRegister(dev_handle, ACC_MSB_Z, &read_buf[0]);
            BMI160_ReadRegister(dev_handle, ACC_LSB_Z, &read_buf[1]);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;
    }
    return axis_value;
}

float BMI160_ReadGyro(i2c_master_dev_handle_t dev_handle, Axis axis){
    float axis_value = 0;
    uint8_t read_buf[2];
    switch(axis){
        case AXIS_X:
            BMI160_ReadRegister(dev_handle, GYR_MSB_X, &read_buf[0]);
            BMI160_ReadRegister(dev_handle, GYR_LSB_X, &read_buf[1]);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Y:
            BMI160_ReadRegister(dev_handle, GYR_MSB_Y, &read_buf[0]);
            BMI160_ReadRegister(dev_handle, GYR_LSB_Y, &read_buf[1]);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;

        case AXIS_Z:
            BMI160_ReadRegister(dev_handle, GYR_MSB_Z, &read_buf[0]);
            BMI160_ReadRegister(dev_handle, GYR_LSB_Z, &read_buf[1]);
            axis_value = ((uint16_t)(read_buf[0] << 8) | read_buf[1]);
            return axis_value;
        break;
    }
    return axis_value;
}

void BMI160_PrintValues(void){
    printf("ACC_X: %f | ACC_Y: %f | ACC_Z: %f\nGYR_X: %f | GYR_Y: %f | GYR_Z: %f\n\n", ACC_X, ACC_Y, ACC_Z, GYR_X, GYR_Y, GYR_Z);
}





