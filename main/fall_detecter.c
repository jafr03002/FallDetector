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

void detector_task(void *args){
    while(1){
        printf("Hello World!\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(&detector_task, "detector", 2048, NULL, 5, NULL); 
}
