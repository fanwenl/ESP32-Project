/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#define ESP_TASK_APP0_PRIO ESP_TASK_PRIO_MIN+3
#define ESP_TASK_APP1_PRIO ESP_TASK_PRIO_MIN+3

static const char* TAG = "HELLO";

void app_cpu1(void *arg);
void app_cpu0(void *arg);

void app_main()
{
    BaseType_t res;
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    res = xTaskCreatePinnedToCore(&app_cpu0, "app_cpu0",
                                  2048, NULL,
                                  ESP_TASK_APP0_PRIO, NULL, 0);
    assert(res == pdTRUE);
    ESP_LOGI(TAG, "Starting task on CPU0.");

    res = xTaskCreatePinnedToCore(&app_cpu1, "app_cpu1",
                                  2048, NULL,
                                  ESP_TASK_APP1_PRIO, NULL, 1);
    assert(res == pdTRUE);
    ESP_LOGI(TAG, "Starting task on CPU1.");
}

void app_cpu1(void *arg)
{
    ESP_LOGI(TAG, "Hello world!->CoreID:%d", xPortGetCoreID());

    while (1)
    {
        ESP_LOGI(TAG,"Hello CPU1->CoreID:%d", xPortGetCoreID());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void app_cpu0(void *arg)
{
    ESP_LOGI(TAG,"Hello world!->CoreID:%d", xPortGetCoreID());

    while (1)
    {
        ESP_LOGI(TAG,"Hello CPU0->CoreID:%d", xPortGetCoreID());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
