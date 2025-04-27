/*
 *  Header file for bmi methods for reading, writing, initializing
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/i2c_master.h"

#define CMD              0x7E

#define ACC_RANGE 0x41
#define GYR_RANGE 0x43

#define ACC_LSB_X 0x12
#define ACC_MSB_X 0x13
#define ACC_LSB_Y 0x14
#define ACC_MSB_Y 0x15
#define ACC_LSB_Z 0x16
#define ACC_MSB_Z 0x17

#define GYR_LSB_X 0x0C
#define GYR_MSB_X 0x0D
#define GYR_LSB_Y 0x0E
#define GYR_MSB_Y 0x0F
#define GYR_LSB_Z 0x10
#define GYR_MSB_Z 0x11

#define DEV_ADDR 0x68

typedef enum{
   AXIS_X,
   AXIS_Y,
   AXIS_Z,
}Axis;

esp_err_t BMI160_Init(i2c_master_dev_handle_t handle);
esp_err_t BMI160_ReadRegister(i2c_master_dev_handle_t handle, uint8_t regg_adress, uint8_t * data, size_t len);
esp_err_t BMI160_WriteRegister(i2c_master_dev_handle_t handle, uint8_t regg_adress, uint8_t data_buf);
int16_t BMI160_ReadAccel(i2c_master_dev_handle_t handle, Axis axis);
int16_t BMI160_ReadGyro(i2c_master_dev_handle_t handle, Axis axis);


